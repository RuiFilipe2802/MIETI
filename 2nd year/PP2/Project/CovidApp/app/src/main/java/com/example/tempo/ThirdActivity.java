package com.example.tempo;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.viewpager.widget.ViewPager;

import android.annotation.SuppressLint;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class ThirdActivity extends AppCompatActivity {

    ArrayList<String> s;
    HashMap<Integer,String> idcid;
    String tMin;
    private String URL1 = "https://api.ipma.pt/open-data/forecast/meteorology/cities/daily/";
    private String id = "";
    private String URL3 = ".json";

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_third);
        Bundle b = getIntent().getExtras();
        idcid = (HashMap<Integer, String>) b.getSerializable("ArrayCid");
        s = new ArrayList<>(idcid.values());
        for(int i=0; i<s.size();i++){
            for(Map.Entry<Integer,String> e : idcid.entrySet()) {
                if (Objects.equals(idcid, e.getValue())) {
                    id = Integer.toString(e.getKey());
                }
            }
            String result = URL1+id+URL3;
            ShowData(result);
        }

        ViewPager viewPager = findViewById(R.id.view_pager);
        ViewPagerAdapter adapter = new ViewPagerAdapter(this, s, tMin);
        viewPager.setAdapter(adapter);


    }

    private void ShowData(String url1){
        RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url1, new Response.Listener<String>() {
            @SuppressLint("SetTextI18n")
            @Override
            public void onResponse(String response) {
                try{
                    JSONObject jsonObject = new JSONObject(response);
                    JSONArray jsonArray = jsonObject.getJSONArray("data");
                    for(int i=0;i<jsonArray.length();i++) {
                        JSONObject temperaturas = jsonArray.getJSONObject(i);
                            tMin = String.valueOf(temperaturas.getDouble("tMin"));
                    }

                }catch (JSONException e){e.printStackTrace();}
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                error.printStackTrace();
            }
        });
        int socketTimeout = 30000;
        RetryPolicy policy = new DefaultRetryPolicy(socketTimeout, DefaultRetryPolicy.DEFAULT_MAX_RETRIES, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
        stringRequest.setRetryPolicy(policy);
        requestQueue.add(stringRequest);

    }
}




