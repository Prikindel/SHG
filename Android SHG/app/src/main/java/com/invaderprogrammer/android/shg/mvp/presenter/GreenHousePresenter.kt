package com.invaderprogrammer.android.shg.mvp.presenter

import android.util.Log
import com.invaderprogrammer.android.shg.di.App
import com.invaderprogrammer.android.shg.mvp.contract.GreenHouseContract
import com.invaderprogrammer.android.shg.rest.*
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.util.concurrent.Executors
import java.util.concurrent.TimeUnit
import javax.inject.Inject
import javax.inject.Named

class GreenHousePresenter : GreenHouseContract.Presenter() {
    @Inject
    lateinit var houseApi: GreenHouseApi

    init {
        App.appComponent.inject(this)
    }

    override fun makeDataTHP() {
        if (!isDetach) {
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
                .doOnComplete {}
                .subscribe({},
                    {
                        view.showErrorMessage("ERROR. The server is not responding")
                        it.printStackTrace()
                    }
                )
            )
            timerFunctionLoop(2000) {
                makeDataTHP()
            }
        }
    }

    override fun getStatus() {
        subscribe(houseApi.getStatus()
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .doOnNext {
                view.setStatus(
                    StatusData(
                        fan     = it.fan,
                        door    = it.door,
                        pump    = it.pump,
                        red     = it.red,
                        green   = it.green,
                        blue    = it.blue,
                        light   = it.light
                    )
                )
            }
            .doOnComplete {
                makeDataTHP()
            }
            .subscribe({},
                {
                    view.showErrorMessage("ERROR. The server is not responding")
                    it.printStackTrace()
                    timerFunctionLoop(2000) {
                        getStatus()
                    }
                }
            )
        )
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


    override fun postPump(open: Int) {
        subscribe(houseApi.setPump(open)
            .subscribeOn(Schedulers.io())
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe({

            }, {
                it.printStackTrace()
            }))
    }

}