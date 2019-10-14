package com.invaderprogrammer.android.shg.di

import com.invaderprogrammer.android.shg.activity.MainActivity
import com.invaderprogrammer.android.shg.fragments.HouseFragment
import com.invaderprogrammer.android.shg.mvp.presenter.HousePresenter
import dagger.Component
import javax.inject.Singleton

@Component(modules = [AppModule::class, RestModule::class, MvpModule::class])
@Singleton
interface AppComponent {

    //View
    fun inject(mainActivity: MainActivity)
    fun inject(houseFragment: HouseFragment)

    //Presenter
    fun inject(presenter: HousePresenter)
}