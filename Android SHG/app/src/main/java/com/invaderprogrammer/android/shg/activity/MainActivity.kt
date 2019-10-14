package com.invaderprogrammer.android.shg.activity

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.Fragment
import com.invaderprogrammer.android.shg.R
import com.invaderprogrammer.android.shg.fragments.HouseFragment

class MainActivity : AppCompatActivity(R.layout.activity_main) {

    private val frag: Fragment = HouseFragment()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (savedInstanceState == null) {
            loadFragment()
        }
    }

    fun loadFragment() {
        supportFragmentManager
            .beginTransaction()
            .replace(R.id.container, frag, null)
            .commit()
    }
}