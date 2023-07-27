package com.example.covidapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Objects;

public class SymptomsActivity extends AppCompatActivity {

    private static final String TAG = "QWERTY";
    TextView welcomeName;
    Button buttonSintomas;
    Button buttonEstadoSaude;
    Button buttonResults;
    ArrayList<String> listSymptoms= new ArrayList<>();
    boolean[] checkedSymptoms;
    String[] listState;
    ArrayList<Integer> userSymptoms = new ArrayList<>();
    ArrayList<String> previousSintomas= new ArrayList<>();
    ArrayList<String> sintomas = new ArrayList<>();
    FirebaseDatabase fDatabase;
    DatabaseReference dReference, sReference;
    String reference,estado;
    double latitude, longitude;
    String name;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_symptoms);

        Intent intent = getIntent();
        reference = Objects.requireNonNull(intent.getExtras()).getString("idNode");
        latitude = intent.getDoubleExtra("latitude", latitude);
        longitude = intent.getDoubleExtra("longitude", longitude);

        welcomeName = findViewById(R.id.welcome_name);
        //listSymptoms = getResources().getStringArray(R.array.symptoms);
        //checkedSymptoms = new boolean[listSymptoms.size()];
        //Log.d(TAG, "TAG"+listSymptoms.size());
        buttonSintomas=findViewById(R.id.button_symptoms);
        buttonEstadoSaude=findViewById(R.id.button_health);
        buttonResults=findViewById(R.id.button_results);
        fDatabase = FirebaseDatabase.getInstance();
        dReference = fDatabase.getReference("User");
        sReference = fDatabase.getReference("Sintomas");

        sReference.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for( DataSnapshot snapshot : dataSnapshot.getChildren()){
                    listSymptoms.add((String) snapshot.getValue());
                }
                checkedSymptoms = new boolean[listSymptoms.size()];
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
            }
        });

        dReference.child(reference).child("nome").addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                name = dataSnapshot.getValue(String.class);
                //Log.d(TAG, "TAG"+name);
                welcomeName.setText("Olá "+name);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

        dReference.child(reference).child("estado").addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                estado = dataSnapshot.getValue(String.class);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
            }
        });


        buttonSintomas.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder mBuilder = new AlertDialog.Builder(SymptomsActivity.this);
                mBuilder.setTitle("Sintomas:");
                mBuilder.setMultiChoiceItems(listSymptoms.toArray(new String[0]), checkedSymptoms, new DialogInterface.OnMultiChoiceClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int position, boolean isChecked) {
                        if (isChecked) {
                            if (!userSymptoms.contains(position)) {
                                userSymptoms.add(position);
                            } else {
                                userSymptoms.remove(position);
                            }
                        }
                    }
                });
                mBuilder.setCancelable(false);
                mBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        for(int i=0;i<userSymptoms.size();i++){
                            sintomas.add(listSymptoms.get(userSymptoms.get(i)));
                            Log.d(TAG, "TAG"+sintomas);
                        }
                        dReference.child(reference).child("sintomas").setValue(sintomas);
                    }
                });

                mBuilder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });

                mBuilder.setNeutralButton("Clear all", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        for(int i=0;i<checkedSymptoms.length;i++){
                            checkedSymptoms[i]= false;
                            userSymptoms.clear();
                            sintomas.clear();
                        }
                        dReference.child(reference).child("sintomas").setValue(sintomas);
                    }
                });
                AlertDialog mDialog = mBuilder.create();
                mDialog.show();
            }
        });

        /*if(sintomas.size()==0){
            getCurrentSymptoms();
        }*/

        buttonEstadoSaude.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                listState = new String[]{"Infeção confirmada","Caso suspeito","Recuperado","Presumidamente saudável"};
                AlertDialog.Builder mBuilder = new AlertDialog.Builder(SymptomsActivity.this);
                mBuilder.setTitle("Estado de Saúde:");
                mBuilder.setSingleChoiceItems(listState, -1, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        estado = listState[which];
                        dialog.dismiss();
                        dReference.child(reference).child("estado").setValue(estado);
                    }
                });

                mBuilder.setNeutralButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                        dReference.child(reference).child("estado").setValue(estado);
                    }
                });
                AlertDialog mDialog = mBuilder.create();
                mDialog.show();
            }
        });

        buttonResults.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                resultsActivity();
            }
        });
    }

    public void getCurrentSymptoms(){
            dReference.child(reference).child("sintomas").addListenerForSingleValueEvent(new ValueEventListener() {
                @Override
                public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                    sintomas = (ArrayList<String>) dataSnapshot.getValue();
                    Log.d(TAG, "TAG"+sintomas);
                }

                @Override
                public void onCancelled(@NonNull DatabaseError databaseError) {
                }
            });
    }

    public void resultsActivity(){
        Intent intent = new Intent(this, Dashboard.class);
        intent.putExtra("idNode", reference);
        intent.putExtra("latitude", latitude);
        intent.putExtra("longitude", longitude);
        intent.putExtra("estado", estado);
        intent.putExtra("sintomas", sintomas);
        startActivity(intent);
    }
}
