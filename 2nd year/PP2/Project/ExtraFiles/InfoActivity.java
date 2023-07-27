package com.example.covidapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Objects;

public class InfoActivity extends AppCompatActivity {

    String titulo;
    TextView title, masculino, feminino, outro, idade1, idade2, idade3;
    int masculino_number, feminino_number, outro_number, idade_1, idade_2, idade_3;
    Button getback;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_info);

        title = findViewById(R.id.title);
        masculino = findViewById(R.id.masculino_number);
        feminino = findViewById(R.id.feminino_number);
        outro = findViewById(R.id.outro_number);
        idade1 = findViewById(R.id.idade1);
        idade2 = findViewById(R.id.idade2);
        idade3 = findViewById(R.id.idade3);
        getback = findViewById(R.id.button_getBack);

        Intent intent = getIntent();
        titulo = Objects.requireNonNull(intent.getExtras()).getString("title");
        masculino_number = intent.getExtras().getInt("masculino");
        feminino_number = intent.getExtras().getInt("feminino");
        outro_number = intent.getExtras().getInt("outro");
        idade_1 = intent.getExtras().getInt("idade1");
        idade_2 = intent.getExtras().getInt("idade2");
        idade_3 = intent.getExtras().getInt("idade3");

        title.setText(titulo);
        masculino.setText(String.valueOf(masculino_number));
        feminino.setText(String.valueOf(feminino_number));
        outro.setText(String.valueOf(outro_number));
        idade1.setText(String.valueOf(idade_1));
        idade2.setText(String.valueOf(idade_2));
        idade3.setText(String.valueOf(idade_3));

        getback.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }
}
