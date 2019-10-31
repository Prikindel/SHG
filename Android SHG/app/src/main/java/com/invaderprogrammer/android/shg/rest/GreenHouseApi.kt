package com.invaderprogrammer.android.shg.rest

import io.reactivex.Observable
import retrofit2.http.GET
import retrofit2.http.Query

interface GreenHouseApi{
    @GET("getdata")
    fun getTempHumPresCOWaterData() : Observable<TempHumPresCOWaterData>

    @GET("light-room")
    fun setLightRoom(
        @Query("red") red: Int,
        @Query("green") green: Int,
        @Query("blue") blue: Int,
        @Query("light") light: Int
    ) : Observable<String>

    @GET("open-door")
    fun setOpenDoor(
        @Query("door") door: Int
    ) : Observable<String>

    @GET("fan")
    fun setFan(
        @Query("fan") fan: Int
    ) : Observable<String>

    @GET("pump")
    fun setPump(
        @Query("pump") pump: Int
    ) : Observable<String>

    @GET("status")
    fun getStatus() : Observable<StatusData>
}