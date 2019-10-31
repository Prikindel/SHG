package com.invaderprogrammer.android.shg.activity

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.Fragment
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.invaderprogrammer.android.shg.R
import com.invaderprogrammer.android.shg.fragments.GreenHouseFragment
import com.invaderprogrammer.android.shg.fragments.HouseFragment
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(R.layout.activity_main) {

    private var frag: Fragment = HouseFragment()

    private val onNavigationItemSelectedListener = BottomNavigationView.OnNavigationItemSelectedListener { item ->
        when (item.itemId) {
            R.id.navigation_house -> {
                frag = HouseFragment()
                loadFragment()
                item.isChecked = item.isChecked
                true
            }
            R.id.navigation_greenhouse -> {
                frag = GreenHouseFragment()
                item.isChecked = item.isChecked
                loadFragment()
                true
            }
        }
        false
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        nav_view.setOnNavigationItemSelectedListener(onNavigationItemSelectedListener)

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