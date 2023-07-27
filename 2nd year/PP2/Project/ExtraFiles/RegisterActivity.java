package com.example.covidapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatCheckBox;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class RegisterActivity extends AppCompatActivity implements AdapterView.OnItemSelectedListener {

    private static final String TAG = "QWERTY";
    EditText rName,rEmail,rPassword,rIdade;
    Button rRegisto;
    Spinner spinner_gender;
    String gender_spinner;
    private FirebaseAuth rAuth;
    FirebaseDatabase fDatabase;
    DatabaseReference dReference;
    String cidade = "";
    String mail;
    double latitude, longitude;
    AppCompatCheckBox checkBox;

    @SuppressLint("WrongViewCast")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        Intent intent = getIntent();
        cidade = Objects.requireNonNull(intent.getExtras()).getString("cidade");
        latitude = intent.getDoubleExtra("latitude", latitude);
        longitude = intent.getDoubleExtra("longitude", longitude);

        rName=findViewById(R.id.editText_name);
        rEmail=findViewById(R.id.editText_email);
        rPassword=findViewById(R.id.editText_password);
        rIdade=findViewById(R.id.editText_idade);
        rRegisto=findViewById(R.id.registo_ok);
        checkBox = findViewById(R.id.checkBox_1);

        checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    rPassword.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                }else {
                    rPassword.setTransformationMethod(PasswordTransformationMethod.getInstance());
                }
            }
        });

        rAuth=FirebaseAuth.getInstance();
        fDatabase = FirebaseDatabase.getInstance();
        dReference = fDatabase.getReference("User");

        spinner_gender=findViewById(R.id.spinnerGender);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,R.array.gender,android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner_gender.setAdapter(adapter);
        spinner_gender.setOnItemSelectedListener(this);

        rRegisto.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String email=rEmail.getText().toString().trim();
                String password=rPassword.getText().toString().trim();

                if(TextUtils.isEmpty(email)){
                    rEmail.setError("Email is required");
                    return;
                }

                if(TextUtils.isEmpty(password)){
                    rPassword.setError("Password is required");
                    return;
                }

                if(password.length()<6){
                    rPassword.setError("Password must have at least 6 characters");
                    return;
                }

                rAuth.createUserWithEmailAndPassword(email,password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            Toast.makeText(RegisterActivity.this, "User Created", Toast.LENGTH_SHORT).show();
                            registo();
                        } else {
                            Toast.makeText(RegisterActivity.this, Objects.requireNonNull(task.getException()).getMessage(), Toast.LENGTH_SHORT).show();
                        }
                    }
                });
            }
        });
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        gender_spinner = parent.getItemAtPosition(position).toString();
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
    }

    private void registo(){
        String email = rEmail.getText().toString();
        String nome = rName.getText().toString();
        int idade = Integer.parseInt(rIdade.getText().toString());
        String gender = gender_spinner;
        String localizacao = cidade;
        ArrayList<String> sintomas= new ArrayList<>();
        String estado = "";
        double latitudeUser = latitude;
        double longitudeUser = longitude;

        String[] mailSplit = email.split("@");
        mail = mailSplit[0];

        User user = new User(email,nome,idade,gender,localizacao,sintomas,estado, latitudeUser, longitudeUser);
        dReference.child(mail).setValue(user);

        Intent intent = new Intent(this, SymptomsActivity.class);
        intent.putExtra("idNode",mail);
        intent.putExtra("latitude", latitude);
        intent.putExtra("longitude", longitude);
        startActivity(intent);
    }
}
