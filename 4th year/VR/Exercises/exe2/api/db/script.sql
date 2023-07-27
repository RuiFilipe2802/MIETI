CREATE DATABASE IF NOT EXISTS
    alunos;
USE alunos;

CREATE TABLE IF NOT EXISTS identificacao (
    name VARCHAR(255),
    number VARCHAR(255),
    email VARCHAR(255),
    PRIMARY KEY(number)
);

INSERT INTO identificacao VALUE('Rui Filipe Ribeiro Freitas','PG47639','pg47639@alunos.uminho.pt');
INSERT INTO identificacao VALUE('Tiago Joao Pereira Ferreira','PG47692','pg47692@alunos.uminho.pt')