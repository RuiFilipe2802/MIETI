package com.example.tempo;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Build;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.viewpager.widget.PagerAdapter;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class ViewPagerAdapter extends PagerAdapter {

    private Context context;
    private ArrayList<String> cids;
    String tMin;


    public ViewPagerAdapter(Context context, ArrayList<String> cids, String tMin) {
        this.context = context;
        this.cids = cids;
        this.tMin = tMin;
    }

    @Override
    public int getCount() {
        return cids.size();
    }

    @Override
    public boolean isViewFromObject(@NonNull View view, @NonNull Object object) {
        return view == object;
    }

    @SuppressLint("SetTextI18n")
    @NonNull
    @Override
    public Object instantiateItem(@NonNull ViewGroup container, int position) {
        View v = LayoutInflater.from(container.getContext()).inflate(R.layout.paginaview ,container, false);
        TextView textView = v.findViewById(R.id.textview_pagina_viewpager);
        textView.setText(cids.get(position));
        TextView textView2 = v.findViewById(R.id.textview_pagina_tmin);
        textView2.setText("Tmin"+tMin);
        container.addView(v);
        return v;
        

    }

    @Override
    public void destroyItem(@NonNull ViewGroup container, int position, @NonNull Object object) {
        container.removeView((View) object);
    }
}
