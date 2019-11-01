package com.invaderprogrammer.android.shg.fragments

import android.annotation.SuppressLint
import android.graphics.Color
import android.graphics.drawable.Drawable
import android.os.Bundle
import android.os.SystemClock
import android.view.View
import android.widget.SeekBar
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.invaderprogrammer.android.shg.R
import com.invaderprogrammer.android.shg.di.App
import com.invaderprogrammer.android.shg.mvp.contract.GreenHouseContract
import com.invaderprogrammer.android.shg.mvp.presenter.GreenHousePresenter
import com.invaderprogrammer.android.shg.rest.LightRoom
import com.invaderprogrammer.android.shg.rest.StatusData
import com.invaderprogrammer.android.shg.rest.TempHumPresCOWaterData
import com.larswerkman.holocolorpicker.ColorPicker
import kotlinx.android.synthetic.main.greenhouse_fragment.*
import kotlinx.android.synthetic.main.greenhouse_fragment.brightness
import kotlinx.android.synthetic.main.greenhouse_fragment.co2_value
import kotlinx.android.synthetic.main.greenhouse_fragment.door_button
import kotlinx.android.synthetic.main.greenhouse_fragment.fan_button
import kotlinx.android.synthetic.main.greenhouse_fragment.humidity_value
import kotlinx.android.synthetic.main.greenhouse_fragment.pressure_value
import kotlinx.android.synthetic.main.greenhouse_fragment.temperature_value
import kotlinx.android.synthetic.main.greenhouse_fragment.water_value
import kotlinx.android.synthetic.main.house_fragment.*
import javax.inject.Inject

class GreenHouseFragment : Fragment(R.layout.greenhouse_fragment), GreenHouseContract.View, ColorPicker.OnColorChangedListener {

    @Inject
    lateinit var presenter: GreenHousePresenter

    private var lightRoom = LightRoom(0, 0, 0, 0)
    private var fanState = 0
    private var doorState = 0
    private var pumpState = 0

    private val sendInterval = 50L
    private var nextSend: Long = 0L


    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        App.appComponent.inject(this)


        presenter.attach(this)
        presenter.getStatus()


        color_picker.addSVBar(svbar_color)
        color_picker.onColorChangedListener = this
        color_picker.setOnColorSelectedListener {
            presenter.postLight(lightRoom)
        }

        brightness.setOnSeekBarChangeListener(seekListener())

        fan_button.setOnClickListener(clickFan())
        door_button.setOnClickListener(clickDoor())
        pump_button.setOnClickListener(clickPump())
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

    private fun clickPump() =
        View.OnClickListener {
            pumpState = if (pumpState == 0) 1 else 0
            presenter.postPump(pumpState)
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
        if (pumpState == 1) {
            pump_button.setBackgroundColor(resources.getColor(R.color.colorButtonOn))
        } else {
            pump_button.setBackgroundColor(resources.getColor(R.color.colorButtonOff))
        }
    }

    private fun setLightView() {
        lightRoom.apply {
            color_picker.oldCenterColor = Color.rgb(red, green, blue)
            color_picker.color = Color.rgb(red, green, blue)
            svbar_color.color = Color.rgb(red, green, blue)
        }
    }

    override fun onColorChanged(color: Int) {
        lightRoom.apply {
            red = Color.red(color)
            green = Color.green(color)
            blue = Color.blue(color)
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
            water_value.text = "$water${resources.getString(R.string.hum_unit)}"
        }
    }

    override fun setStatus(data: StatusData) {
        data.apply {
            fanState = fan
            doorState = door
            pumpState = pump
            lightRoom.red = red
            lightRoom.blue = blue
            lightRoom.green = green
            lightRoom.light = light
        }
        setLightView()
        lightRoom.apply {
            brightness.progress = light
            brightness_text.text = light.toString()
        }
        setColorButton()
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