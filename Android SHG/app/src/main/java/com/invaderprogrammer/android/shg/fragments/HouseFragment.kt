package com.invaderprogrammer.android.shg.fragments

import android.annotation.SuppressLint
import android.graphics.Color
import android.os.Bundle
import android.os.SystemClock
import android.view.View
import android.widget.SeekBar
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.invaderprogrammer.android.shg.R
import com.invaderprogrammer.android.shg.di.App
import com.invaderprogrammer.android.shg.mvp.contract.HouseContract
import com.invaderprogrammer.android.shg.mvp.presenter.HousePresenter
import com.invaderprogrammer.android.shg.rest.LightRoom
import com.invaderprogrammer.android.shg.rest.TempHumPresCOWaterData
import com.larswerkman.holocolorpicker.ColorPicker
import kotlinx.android.synthetic.main.house_fragment.*
import javax.inject.Inject

class HouseFragment : Fragment(R.layout.house_fragment), HouseContract.View, ColorPicker.OnColorChangedListener {

    @Inject
    lateinit var presenter: HousePresenter

    private var lightRoom = LightRoom(0, 0, 0, 0)
    private var fanState = 0
    private var doorState = 0

    private val sendInterval = 150L
    private var nextSend: Long = 0L


    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        App.appComponent.inject(this)
        presenter.attach(this)
        presenter.makeDataTHP()

        color_picker.addSVBar(svbar_color)
        color_picker.onColorChangedListener = this
        color_picker.setOnColorSelectedListener {
            presenter.postLight(lightRoom)
        }

        brightness.setOnSeekBarChangeListener(seekListener())

        fan_button.setOnClickListener(clickFan())
        door_button.setOnClickListener(clickDoor())
    }

    private fun seekListener() =
        object : SeekBar.OnSeekBarChangeListener {
            fun whenSeek(seekBar: SeekBar?, value: Int) {
                lightRoom.apply {
                    when (seekBar) {
                        brightness  -> {
                            light = value
                            brightness_text.text = value.toString()
                        }
                    }
                }
            }

            override fun onProgressChanged(seekBar: SeekBar?, i: Int, b: Boolean) {
                whenSeek(seekBar, i)
                if (nextSend < SystemClock.uptimeMillis()) {
                    presenter.postLight(lightRoom)
                    nextSend = SystemClock.uptimeMillis() + sendInterval
                }
                setLightView()
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
                nextSend = 0
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                presenter.postLight(lightRoom)
            }

        }

    private fun clickFan() =
        View.OnClickListener {
            fanState = if (fanState == 0) 1 else 0
            presenter.postFan(fanState)
            setColorButton()
        }

    private fun clickDoor() =
        View.OnClickListener {
            doorState = if (doorState == 0) 90 else 0
            presenter.postDoor(doorState)
            setColorButton()
        }

    private fun setColorButton() {
        if (fanState == 1) {
            fan_button.setBackgroundColor(resources.getColor(R.color.colorButtonOn))
        } else {
            fan_button.setBackgroundColor(resources.getColor(R.color.colorButtonOff))
        }
        if (doorState != 0) {
            door_button.setBackgroundColor(resources.getColor(R.color.colorButtonOn))
        } else {
            door_button.setBackgroundColor(resources.getColor(R.color.colorButtonOff))
        }
    }

    fun setLightView() {
        lightRoom.apply {
            color_picker.oldCenterColor = Color.rgb(red, green, blue)
            color_picker.color = Color.rgb(red, green, blue)
            svbar_color.color = Color.rgb(red, green, blue)
        }
    }

    override fun onColorChanged(color: Int) {
        lightRoom.apply {
            red = Color.red(svbar_color.color)
            green = Color.green(svbar_color.color)
            blue = Color.blue(svbar_color.color)
        }
        if (nextSend < SystemClock.uptimeMillis()) {
            presenter.postLight(lightRoom)
            nextSend = SystemClock.uptimeMillis() + sendInterval
        }
        color_picker.oldCenterColor = color
    }

    @SuppressLint("SetTextI18n")
    override fun updateDataTHP(data: TempHumPresCOWaterData) {
        data.apply {
            temperature_value.text = "${temperature}${resources.getString(R.string.temp_unit)}"
            humidity_value.text = "$humidity${resources.getString(R.string.hum_unit)}"
            pressure_value.text = "$pressure ${resources.getString(R.string.pres_unit)}"
            co2_value.text = co2.toString()
            water_value.text = water.toString()
        }
    }

    override fun showErrorMessage(error: String?) {
        Toast.makeText(context, error, Toast.LENGTH_SHORT).show()
    }

    override fun refresh() {

    }

    override fun onResume() {
        super.onResume()
        presenter.attach(this)
    }

    override fun onPause() {
        super.onPause()
        presenter.detach()
    }
}