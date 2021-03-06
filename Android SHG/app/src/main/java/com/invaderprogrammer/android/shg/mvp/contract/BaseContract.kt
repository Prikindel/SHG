package com.invaderprogrammer.android.shg.mvp.contract

import io.reactivex.disposables.CompositeDisposable
import io.reactivex.disposables.Disposable

class BaseContract {
    interface View

    abstract class Presenter<V : View> {
        protected var isDetach = false
        private val subscriptions = CompositeDisposable()
        protected lateinit var view: V

        fun subscribe(subscription: Disposable) {
            subscriptions.add(subscription)
        }

        fun unsubscribe() {
            subscriptions.clear()
        }

        fun attach(view: V) {
            this.view = view
            isDetach = false
        }

        fun detach() {
            unsubscribe()
            isDetach = true
        }
    }
}