package com.invaderprogrammer.android.shg.di

import com.google.gson.Gson
import com.google.gson.GsonBuilder
import com.invaderprogrammer.android.shg.rest.GreenHouseApi
import com.invaderprogrammer.android.shg.rest.HouseApi
import dagger.Module
import dagger.Provides
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import retrofit2.Retrofit
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
import retrofit2.converter.gson.GsonConverterFactory
import java.util.concurrent.TimeUnit
import javax.inject.Named
import javax.inject.Singleton

@Module
class RestModule {

    @Provides
    @Singleton
    fun provideGson(): Gson =
        GsonBuilder()
            .setLenient()
            .create()

    @Provides
    @Singleton
    fun provideOkHttpClient() : OkHttpClient =
        OkHttpClient.Builder()
            .addInterceptor(HttpLoggingInterceptor().apply {
                level = HttpLoggingInterceptor.Level.BODY
            })
            .connectTimeout(60, TimeUnit.SECONDS)
            .build()

    @Provides
    @Singleton
    fun provideSHGRetrofitHouse(gson: Gson, okHttpClient: OkHttpClient): HouseApi =
        Retrofit.Builder()
            .baseUrl("http://192.168.10.177/")
            .addConverterFactory(GsonConverterFactory.create(gson))
            .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
            .client(okHttpClient)
            .build()
            .create(HouseApi::class.java)

    @Provides
    @Singleton
    fun provideSHGRetrofitGreenHouse(gson: Gson, okHttpClient: OkHttpClient): GreenHouseApi =
        Retrofit.Builder()
            .baseUrl("http://192.168.10.155/")
            .addConverterFactory(GsonConverterFactory.create(gson))
            .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
            .client(okHttpClient)
            .build()
            .create(GreenHouseApi::class.java)
}