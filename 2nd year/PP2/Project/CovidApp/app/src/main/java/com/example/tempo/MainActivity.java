package com.example.tempo;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Spinner;
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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;




public class MainActivity extends AppCompatActivity implements Serializable{
     Spinner spinner;
     Button botao;
     Button botao2act;
     ImageView sol1;
     ImageView chuva1;
     ImageView sol2;
     ImageView chuva2;
     ImageView sol3;
     ImageView chuva3;
     private String URL1 = "https://api.ipma.pt/open-data/forecast/meteorology/cities/daily/";
     private String id = " ";
     private String URL3 = ".json";
     private String tmin1 = " ";
     private String tmin2 = " ";
     private String tmin3 = " ";
     private String tmax1 = " ";
     private String tmax2 = " ";
     private String tmax3 = " ";
     private String prec1 = " ";
     private String prec2 = " ";
     private String prec3 = " ";
     private String local = " ";
     private ArrayList<String> cidsSp = new ArrayList<>();
     private HashMap<Integer,String> idCidades = new HashMap<>();



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        spinner = findViewById(R.id.sp_local);
        botao = findViewById(R.id.ok_button);
        botao2act = findViewById(R.id.button_secondAc);
        sol1 = findViewById(R.id.imagSol1);
        chuva1 = findViewById(R.id.imagChuva1);
        sol2 = findViewById(R.id.imagSol2);
        chuva2 = findViewById(R.id.imagChuva2);
        sol3 = findViewById(R.id.imagSol3);
        chuva3 = findViewById(R.id.imagChuva3);

        String URL = "https://api.ipma.pt/open-data/distrits-islands.json";
        AddSpinnerInfo(URL);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                local = spinner.getItemAtPosition(spinner.getSelectedItemPosition()).toString();
            }
            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                // DO Nothing here
            }
        });
        botao.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.KITKAT)
            @Override
            public void onClick(View v) {
                for(Map.Entry<Integer,String> e : idCidades.entrySet()) {
                    if (Objects.equals(local, e.getValue())) {
                        id = Integer.toString(e.getKey());
                    }
                }
                String result = URL1+id+URL3;
                ShowData(result);
            }
        });

        botao2act.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openTempoAct();
            }
        });
    }
    private void AddSpinnerInfo(String url) {
        RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                try{
                    JSONObject jsonObject = new JSONObject(response);
                        JSONArray jsonArray = jsonObject.getJSONArray("data");
                        for(int i=0;i<jsonArray.length();i++){
                            JSONObject nome = jsonArray.getJSONObject(i);
                            String cid = nome.getString("local");
                            int id = nome.getInt("globalIdLocal");
                            idCidades.put(id,cid);
                            cidsSp.add(cid);
                        }
                    spinner.setAdapter(new ArrayAdapter<>(MainActivity.this, android.R.layout.simple_spinner_dropdown_item, cidsSp));
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
                      if(i==0) {
                          tmin1 = String.valueOf(temperaturas.getDouble("tMin"));
                          TextView tmini =findViewById(R.id.tempMin1);
                          tmini.setText("tMin:"+tmin1+"º");
                          tmax1 = String.valueOf(temperaturas.getDouble("tMax"));
                          TextView tmaxi =findViewById(R.id.tempMax1);
                          tmaxi.setText("tMax:"+tmax1+"º");
                          prec1 = String.valueOf(temperaturas.getDouble("precipitaProb"));
                          double precIm1 = temperaturas.getDouble("precipitaProb");
                          TextView preci =findViewById(R.id.precP1);
                          preci.setText("PrecipitaProb:\n"+prec1+"%");
                          if(precIm1 < 20) {
                              chuva1.setVisibility(View.GONE);
                              sol1.setImageResource(R.drawable.sol);
                          }else{
                              chuva1.setVisibility(View.VISIBLE);
                              chuva1.setImageResource(R.drawable.chuva);
                          }
                      }else if(i==1) {
                           tmin2 = String.valueOf(temperaturas.getDouble("tMin"));
                           TextView tmini =findViewById(R.id.tempMin2);
                           tmini.setText("tMin:"+tmin2+"º");
                           tmax2 = String.valueOf(temperaturas.getDouble("tMax"));
                           TextView tmaxi = findViewById(R.id.tempMax2);
                           tmaxi.setText("tMax:"+tmax2+"º");
                           prec2 = String.valueOf(temperaturas.getDouble("precipitaProb"));
                           double precIm2 = temperaturas.getDouble("precipitaProb");
                           TextView preci =findViewById(R.id.precP2);
                           preci.setText("PrecipitaProb:\n" + prec2 + "%");
                           if(precIm2 < 20) {
                               chuva2.setVisibility(View.GONE);
                               sol2.setImageResource(R.drawable.sol);
                           }else{
                               chuva2.setVisibility(View.VISIBLE);
                               chuva2.setImageResource(R.drawable.chuva);
                           }

                       }else if(i==2) {
                           tmin3 = String.valueOf(temperaturas.getDouble("tMin"));
                           TextView tmini =findViewById(R.id.tempMin3);
                           tmini.setText("tMin:"+tmin3+"º");
                           tmax3 = String.valueOf(temperaturas.getDouble("tMax"));
                           TextView tmaxi =findViewById(R.id.tempMax3);
                           tmaxi.setText("tMax:"+tmax3+"º");
                           prec3 = String.valueOf(temperaturas.getDouble("precipitaProb"));
                           double precIm3 = temperaturas.getDouble("precipitaProb");
                           TextView preci =findViewById(R.id.precP3);
                           preci.setText("PrecipitaProb:\n"+prec3+"%");
                           if(precIm3 < 20) {
                               chuva3.setVisibility(View.GONE);
                               sol3.setImageResource(R.drawable.sol);
                           }else{
                               chuva3.setVisibility(View.VISIBLE);
                               chuva3.setImageResource(R.drawable.chuva);

                           }
                       }
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

   public void openTempoAct(){
        Intent intent = new Intent(this, SecondActivity.class);
        Bundle bundle = new Bundle();
        bundle.putSerializable("ArrayCids", idCidades);
        intent.putExtras(bundle);
       startActivity(intent);
    }


}