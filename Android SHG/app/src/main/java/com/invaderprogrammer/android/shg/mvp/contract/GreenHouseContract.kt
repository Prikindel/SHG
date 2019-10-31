package com.invaderprogrammer.android.shg.mvp.contract

import com.invaderprogrammer.android.shg.rest.LightRoom
import com.invaderprogrammer.android.shg.rest.StatusData
import com.invaderprogrammer.android.shg.rest.TempHumPresCOWaterData

class GreenHouseContract {
    interface View : BaseContract.View {
        fun updateDataTHP(data: TempHumPresCOWaterData)
        fun setStatus(data: StatusData)
        fun showErrorMessage(error: String?)
        fun refresh()
    }

    abstract class Presenter : BaseContract.Presenter<View>() {
        abstract fun makeDataTHP()
        abstract fun refreshDataTHP()
        abstract fun timerFunctionLoop(timer: Long, function: () -> (Unit))
        abstract fun postLight(lightRoom: LightRoom)
        abstract fun postDoor(open: Int)
        abstract fun postFan(open: Int)
        abstract fun postPump(open: Int)
        abstract fun getStatus()
    }
}
