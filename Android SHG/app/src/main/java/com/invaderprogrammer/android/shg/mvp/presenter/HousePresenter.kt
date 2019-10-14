package com.invaderprogrammer.android.shg.mvp.presenter

import com.invaderprogrammer.android.shg.di.App
import com.invaderprogrammer.android.shg.mvp.contract.HouseContract
import com.invaderprogrammer.android.shg.rest.HouseApi
import com.invaderprogrammer.android.shg.rest.LightRoom
import com.invaderprogrammer.android.shg.rest.TempHumPresCOWaterData
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.util.concurrent.Executors
import java.util.concurrent.TimeUnit
import javax.inject.Inject
import javax.xml.datatype.DatatypeConstants.SECONDS



class HousePresenter : HouseContract.Presenter() {

    @Inject
    lateinit var houseApi: HouseApi

    init {
        App.appComponent.inject(this)
    }

    override fun makeDataTHP() {
        subscribe(houseApi.getTempHumPresCOWaterData()
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .doOnNext {
                view.updateDataTHP(
                    TempHumPresCOWaterData(
                        temperature = it.temperature,
                        humidity = it.humidity,
                        pressure = it.pressure,
                        co2 = it.co2,
                        water = it.water
                    )
                )
            }
            .doOnComplete{}
            .subscribe({},
                {
                    view.showErrorMessage("ERROR. The server is not responding")
                    it.printStackTrace()
                }
            )
        )
        timerFunctionLoop(2000){
            makeDataTHP()
        }
    }

    override fun refreshDataTHP() {
        view.refresh()
        makeDataTHP()
    }

    override fun postLight(lightRoom: LightRoom) {
        subscribe(houseApi.setLightRoom(
                lightRoom.red,
                lightRoom.green,
                lightRoom.blue,
                lightRoom.light
            )
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe({

            }, {
                it.printStackTrace()
            }))
    }

    override fun postDoor(open: Int) {
        subscribe(houseApi.setOpenDoor(open)
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe({

            }, {
                it.printStackTrace()
            }))
    }

    override fun postFan(open: Int) {
        subscribe(houseApi.setFan(open)
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe({

            }, {
                it.printStackTrace()
            }))
    }

    override fun timerFunctionLoop(timer: Long, function: () -> Unit) {
        Executors.newSingleThreadScheduledExecutor()
            .scheduleAtFixedRate(Runnable { function() }, 2, timer, TimeUnit.SECONDS)
    }
}