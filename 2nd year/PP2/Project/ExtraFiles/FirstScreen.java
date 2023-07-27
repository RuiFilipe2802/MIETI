package com.example.covidapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Location;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.tasks.OnSuccessListener;

public class FirstScreen extends AppCompatActivity implements GetLocation.OnTaskCompleted {

    private static final int REQUEST_LOCATION_PERMISSION = 1;
    private static final String TAG = "QWERTY" ;
    private FusedLocationProviderClient mFusedLocationClient;
    //TextView cidade_location;
    Button registoButtton,loginButton,resultadosButton;
    String cidade = "";
    double latitude, longitude;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_next);

        registoButtton = findViewById(R.id.button_registo);
        loginButton = findViewById(R.id.button_login);
        resultadosButton = findViewById(R.id.resultados_button);
        //cidade_location = findViewById(R.id.location_name);
        mFusedLocationClient = LocationServices.getFusedLocationProviderClient(this);
        getLocation();

        loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loginActivity();
            }
        });

        registoButtton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                registerActivity();
            }
        });

        resultadosButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                resultadosActivity();
            }
        });
    }

    public void getLocation(){
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION)!= PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, REQUEST_LOCATION_PERMISSION);
        } else {
            mFusedLocationClient.getLastLocation().addOnSuccessListener(new OnSuccessListener<Location>() {
                @Override
                public void onSuccess(Location location) {
                    new GetLocation(FirstScreen.this, FirstScreen.this).execute(location);
                }
            });
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == REQUEST_LOCATION_PERMISSION) {// If the permission is granted, get the location,
            // otherwise, show a Toast
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                getLocation();
            } else {
                Toast.makeText(FirstScreen.this, "Location Permission Denied", Toast.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    public void onTaskCompleted(String result) {
        // Update the UI
        String[] message = result.split(",");
        String[] coordenates = message[1].split("_");
        cidade= message[0];
        latitude = Double.parseDouble(coordenates[0]);
        longitude = Double.parseDouble(coordenates[1]);
        //cidade_location.setText(getString(R.string.address_text, cidade));
        resultadosButton.setText(String.format("%s %s", getString(R.string.resultados_button), cidade));
    }

    public void registerActivity(){
        Intent intent = new Intent(this, RegisterActivity.class);
        intent.putExtra("cidade",cidade);
        intent.putExtra("latitude",latitude);
        intent.putExtra("longitude",longitude);
        startActivity(intent);
    }

    public void loginActivity(){
        Intent intent = new Intent(this, LoginActivity.class);
        intent.putExtra("latitude",latitude);
        intent.putExtra("longitude",longitude);
        startActivity(intent);
    }

    public void resultadosActivity(){
        Intent intent = new Intent(this, DashboardActivity.class);
        intent.putExtra("cidade",cidade);
        startActivity(intent);
    }
}
