package com.example.covidapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import java.util.ArrayList;
import java.util.Objects;

public class DashboardActivity extends AppCompatActivity {

    private static final String TAG = "QWERTY";
    TextView comSintomas,semSintomas,infetados,suspeitos, cidadeView;
    //ImageView logoApp;
    FirebaseDatabase fDatabase;
    DatabaseReference dReference;
    int casosComSintomas = 0;
    int casosSemSintomas = 0;
    int casosInfetados = 0;
    int casosSuspeitos = 0;
    Button getBackEcra;
    String cidade;
    int masculinoSS,femininoSS, outroSS, masculinoCS, femininoCS, outroCS=0;
    int masculinoI, femininoI, outroI, masculinoS, femininoS, outroS=0;
    int idade1SS, idade2SS, idade3SS, idade1CS, idade2CS, idade3CS=0;
    int idade1I, idade2I, idade3I, idade1S, idade2S, idade3S=0;
    ProgressBar pBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dashboard);

        fDatabase = FirebaseDatabase.getInstance();
        dReference = fDatabase.getReference("User");

        comSintomas=findViewById(R.id.com_sintomas_number);
        semSintomas=findViewById(R.id.sem_sintomas_number);
        infetados=findViewById(R.id.infetados_number);
        suspeitos=findViewById(R.id.suspeitos_number);
        getBackEcra=findViewById(R.id.button_update);
        getBackEcra.setText(R.string.backButton);
        cidadeView = findViewById(R.id.text_city);
        //logoApp = findViewById(R.id.image_App);
        pBar = findViewById(R.id.progressBar);
        pBar.setVisibility(View.VISIBLE);

        Intent intent = getIntent();
        cidade = Objects.requireNonNull(intent.getExtras()).getString("cidade");

        cidadeView.setText(cidade);
        //logoApp.setVisibility(View.GONE);

        dReference.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot data : dataSnapshot.getChildren()) {
                    ArrayList<String> sintomas = (ArrayList<String>) data.child("sintomas").getValue();
                    String estado = String.valueOf(data.child("estado").getValue());
                    String localidade = String.valueOf(data.child("localizacao").getValue());
                    String gender = String.valueOf(data.child("gender").getValue());
                    String age = String.valueOf(data.child("idade").getValue());
                    int idade = Integer.parseInt(age);
                    if (cidade.equals(localidade)) {
                        assert sintomas != null;
                        if (sintomas.contains("Sem sintomas")) {
                            casosSemSintomas++;
                            if(gender.equals("Masculino")){
                                masculinoSS++;
                            }
                            if(gender.equals("Feminino")){
                                femininoSS++;
                            }
                            if(gender.equals("Outro")){
                                outroSS++;
                            }
                            if(idade<20){
                                idade1SS++;
                            }
                            if(idade>=20 && idade<50){
                                idade2SS++;
                            }
                            if(idade>=50){
                                idade3SS++;
                            }
                        }
                        if (sintomas.size() > 0 && !sintomas.contains("Sem sintomas")) {
                            casosComSintomas++;
                            if(gender.equals("Masculino")){
                                masculinoCS++;
                            }
                            if(gender.equals("Feminino")){
                                femininoCS++;
                            }
                            if(gender.equals("Outro")){
                                outroCS++;
                            }
                            if(idade<20){
                                idade1CS++;
                            }
                            if(idade>=20 && idade<50){
                                idade2CS++;
                            }
                            if(idade>=50){
                                idade3CS++;
                            }
                        }
                        if (estado.contains("Infeção confirmada")) {
                            casosInfetados++;
                            if(gender.equals("Masculino")){
                                masculinoI++;
                            }
                            if(gender.equals("Feminino")){
                                femininoI++;
                            }
                            if(gender.equals("Outro")){
                                outroI++;
                            }
                            if(idade<20){
                                idade1I++;
                            }
                            if(idade>=20 && idade<50){
                                idade2I++;
                            }
                            if(idade>=50){
                                idade3I++;
                            }
                        }
                        if (sintomas.size() > 2 && !sintomas.contains("Sem sintomas")) {
                            casosSuspeitos++;
                            if(gender.equals("Masculino")){
                                masculinoS++;
                            }
                            if(gender.equals("Feminino")){
                                femininoS++;
                            }
                            if(gender.equals("Outro")){
                                outroS++;
                            }
                            if(idade<20){
                                idade1S++;
                            }
                            if(idade>=20 && idade<50){
                                idade2S++;
                            }
                            if(idade>=50){
                                idade3S++;
                            }
                        }
                    }
                    semSintomas.setText(String.valueOf(casosSemSintomas));
                    comSintomas.setText(String.valueOf(casosComSintomas));
                    infetados.setText(String.valueOf(casosInfetados));
                    suspeitos.setText(String.valueOf(casosSuspeitos));
                    Log.d(TAG, "TAG"+casosComSintomas+casosSemSintomas+casosInfetados+casosSuspeitos);
                }
                pBar.setVisibility(View.GONE);
            }
            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
            }
        });

        getBackEcra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        comSintomas.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                infoActivityComSintomas();
            }
        });

        semSintomas.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                infoActivitySemSintomas();
            }
        });

        infetados.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                infoActivityInfetados();
            }
        });

        suspeitos.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                infoActivitySuspeitos();
            }
        });
    }

    public void infoActivityComSintomas(){
        Intent intent = new Intent(this, InfoActivity.class);
        intent.putExtra("title", "Com Sintomas");
        intent.putExtra("masculino", masculinoCS);
        intent.putExtra("feminino",femininoCS);
        intent.putExtra("outro", outroCS);
        intent.putExtra("idade1", idade1CS);
        intent.putExtra("idade2", idade2CS);
        intent.putExtra("idade3", idade3CS);
        startActivity(intent);
    }

    public void infoActivitySemSintomas(){
        Intent intent = new Intent(this, InfoActivity.class);
        intent.putExtra("title", "Sem Sintomas");
        intent.putExtra("masculino", masculinoSS);
        intent.putExtra("feminino",femininoSS);
        intent.putExtra("outro", outroSS);
        intent.putExtra("idade1", idade1SS);
        intent.putExtra("idade2", idade2SS);
        intent.putExtra("idade3", idade3SS);
        startActivity(intent);
    }

    public void infoActivityInfetados(){
        Intent intent = new Intent(this, InfoActivity.class);
        intent.putExtra("title", "Infetados");
        intent.putExtra("masculino", masculinoI);
        intent.putExtra("feminino",femininoI);
        intent.putExtra("outro", outroI);
        intent.putExtra("idade1", idade1I);
        intent.putExtra("idade2", idade2I);
        intent.putExtra("idade3", idade3I);
        startActivity(intent);
    }
    public void infoActivitySuspeitos(){
        Intent intent = new Intent(this, InfoActivity.class);
        intent.putExtra("title", "Suspeitos");
        intent.putExtra("masculino", masculinoS);
        intent.putExtra("feminino",femininoS);
        intent.putExtra("outro", outroS);
        intent.putExtra("idade1", idade1S);
        intent.putExtra("idade2", idade2S);
        intent.putExtra("idade3", idade3S);
        startActivity(intent);
    }
}
