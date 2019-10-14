package com.invaderprogrammer.android.shg.di

import com.invaderprogrammer.android.shg.mvp.presenter.HousePresenter
import dagger.Module
import dagger.Provides
import javax.inject.Singleton

@Module
class MvpModule {

    @Provides
    @Singleton
    fun provideHousePresenter(): HousePresenter = HousePresenter()
}