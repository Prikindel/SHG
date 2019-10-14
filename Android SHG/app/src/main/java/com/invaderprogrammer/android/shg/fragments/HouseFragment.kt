package com.invaderprogrammer.android.shg.fragments

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
import kotlinx.android.synthetic.main.house_fragment.*
import javax.inject.Inject

class HouseFragment : Fragment(R.layout.house_fragment), HouseContract.View {

    @Inject
    lateinit var presenter: HousePresenter

    private var lightRoom = LightRoom(0, 0, 0, 0)
    private var fanState = 0
    private var doorState = 0

    private val sendInterval = 300L
    private var nextSend: Long = 0L


    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        App.appComponent.inject(this)
        presenter.attach(this)
        presenter.makeDataTHP()

        red_value.setOnSeekBarChangeListener(seekListener())
        green_value.setOnSeekBarChangeListener(seekListener())
        blue_value.setOnSeekBarChangeListener(seekListener())
        brightness.setOnSeekBarChangeListener(seekListener())

        fan_button.setOnClickListener(clickFan())
        door_button.setOnClickListener(clickDoor())
    }

    private fun seekListener() =
        object : SeekBar.OnSeekBarChangeListener {
            fun whenSeek(seekBar: SeekBar?, value: Int) {
                lightRoom.apply {
                    when (seekBar) {
                        red_value   -> red   = value
                        green_value -> green = value
                        blue_value  -> blue  = value
                        brightness  -> light = value
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
                setLightView()
            }

        }

    private fun clickFan() =
        View.OnClickListener {
            fanState = if (fanState == 0) 1 else 0
            presenter.postFan(fanState)
        }

    private fun clickDoor() =
        View.OnClickListener {
            doorState = if (doorState == 0) 90 else 0
            presenter.postDoor(doorState)
        }

    fun setLightView() {
        lightRoom.apply {
            color_light.setBackgroundColor(Color.rgb(red, green, blue))
        }
    }

    override fun updateDataTHP(data: TempHumPresCOWaterData) {
        data.apply {
            temperature_value.text = temperature.toString()
            humidity_value.text = humidity.toString()
            pressure_value.text = pressure.toString()
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