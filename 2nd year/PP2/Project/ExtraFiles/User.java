package com.example.covidapp;

import java.util.ArrayList;

public class User {
    private String email;
    private String nome;
    private int idade;
    private String gender;
    private String localizacao;
    private ArrayList<String> sintomas;
    private String estado;
    private double latitude;
    private double longitude;

    public User(String email, String nome, int idade, String gender, String localizacao, ArrayList<String> sintomas, String estado, double latitude, double longitude) {
        this.email = email;
        this.nome = nome;
        this.idade = idade;
        this.gender = gender;
        this.localizacao = localizacao;
        this.sintomas = sintomas;
        this.estado = estado;
        this.latitude = latitude;
        this.longitude = longitude;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public int getIdade() {
        return idade;
    }

    public void setIdade(int idade) {
        this.idade = idade;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getLocalizacao() {
        return localizacao;
    }

    public void setLocalizacao(String localizacao) {
        this.localizacao = localizacao;
    }

    public ArrayList<String> getSintomas() {
        return sintomas;
    }

    public void setSintomas(ArrayList<String> sintomas) {
        this.sintomas = sintomas;
    }

    public String getEstado() {
        return estado;
    }

    public void setEstado(String estado) {
        this.estado = estado;
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }
}
