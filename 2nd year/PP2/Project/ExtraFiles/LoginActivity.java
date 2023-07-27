package com.example.covidapp;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatCheckBox;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;

import java.util.ArrayList;

public class LoginActivity extends AppCompatActivity {

    private static final String TAG = "QWERTY";
    EditText lEmail,lPassword;
    Button lLogin;
    FirebaseAuth lAuth;
    String mail;
    double latitude, longitude;
    AppCompatCheckBox checkBox;
    ProgressBar progressBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        lEmail = findViewById(R.id.editText_email_login);
        lPassword = findViewById(R.id.editText_password_login);
        lAuth = FirebaseAuth.getInstance();
        lLogin=findViewById(R.id.login_ok);
        checkBox = findViewById(R.id.checkbox);
        progressBar = findViewById(R.id.progressBar2);

        Intent intent = getIntent();
        latitude = intent.getDoubleExtra("latitude", latitude);
        longitude = intent.getDoubleExtra("longitude", longitude);

        checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    lPassword.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                }else {
                    lPassword.setTransformationMethod(PasswordTransformationMethod.getInstance());
                }
            }
        });

        lLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                progressBar.setVisibility(View.VISIBLE);
                String email = lEmail.getText().toString().trim();
                String password = lPassword.getText().toString().trim();

                if(TextUtils.isEmpty(email)){
                    lEmail.setError("Email is required");
                    return;
                }

                if(TextUtils.isEmpty(password)){
                    lPassword.setError("Password is required");
                    return;
                }

                if(password.length()<6){
                    lPassword.setError("Password must have at least 6 characters");
                    return;
                }

                lAuth.signInWithEmailAndPassword(email,password).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if(task.isSuccessful()){
                            login();
                        } else {
                            Toast.makeText(LoginActivity.this, task.getException().getMessage(), Toast.LENGTH_SHORT).show();
                        }
                    }
                });
            }
        });
    }

    public void login(){
        String email = lEmail.getText().toString();
        String[] mailSplit = email.split("@");
        mail = mailSplit[0];

        Intent intent = new Intent(this, SymptomsActivity.class);
        intent.putExtra("idNode",mail);
        intent.putExtra("latitude", latitude);
        intent.putExtra("longitude", longitude);
        startActivity(intent);
    }
}
