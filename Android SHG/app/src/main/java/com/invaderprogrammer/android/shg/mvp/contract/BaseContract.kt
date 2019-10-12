package com.invaderprogrammer.android.shg.mvp.contract

class BaseContract {
    interface View

    abstract class Presenter<V : View> {
        protected lateinit var view: V

        fun attach(view: V) {
            this.view = view
        }
    }
}