package com.invaderprogrammer.android.shg.rest

data class TempHumPresCOWaterData(
    val temperature: Int,
    val humidity: Int,
    val pressure: Int,
    val co2: Int,
    val water: Int
)