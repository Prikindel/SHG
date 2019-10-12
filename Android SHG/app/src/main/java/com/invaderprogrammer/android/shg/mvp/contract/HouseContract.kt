package com.invaderprogrammer.android.shg.mvp.contract

class HouseContract {
    interface View : BaseContract.View {
        fun updateDataTHP()
        fun showErrorMessage(error: String?)
        fun refresh()
    }

    abstract class Presenter : BaseContract.Presenter<View>() {

    }
}