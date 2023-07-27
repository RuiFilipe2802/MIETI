const express = require("express");
const mysql = require("mysql");
const dotenv = require('dotenv');
const path = require('path');
const cookieParser = require('cookie-parser');

dotenv.config({ path: './.env' })

const app = express();

const db = mysql.createConnection({
    host: process.env.DATABASE_HOST,
    user: process.env.DATABASE_USER,
    password: process.env.DATABASE_PASSWORD,
    // database: process.env.DATABASE,
});

const publicDirectory = path.join(__dirname, './public');
app.use(express.static(publicDirectory));

//parse URL encoded bodies (as sent by HTML forms)
app.use(express.urlencoded({ extended: false }))
//parse JSON bodies
app.use(express.json());

app.use(cookieParser());

app.set('view engine', 'hbs');

db.connect((err) => {
    if (err) {
        console.log(err)
    } else {
        console.log("MYSQL Connected...")
    }
})
const sql =
    'CREATE DATABASE IF NOT EXISTS db_auth;'
db.query(sql, function (err, result) {
    if (err) throw err;
    console.log("VAMOOOOS")
});

const sql2 =
    'USE db_auth\n;'
db.query(sql2, function (err, result) {
    if (err) throw err;
    console.log("VAMOOOOS2")
});

const sql3 =
    'CREATE TABLE IF NOT EXISTS user ( id int NOT NULL AUTO_INCREMENT,name VARCHAR(255),email VARCHAR(255), password VARCHAR(255),role VARCHAR(255),PRIMARY KEY(id) );'
db.query(sql3, function (err, result) {
    if (err) throw err;
    console.log("VAMOOOOS3")
});


//Define Routes
app.use('/', require('./routes/pages'));
app.use('/auth', require('./routes/auth'));

app.listen(5000, () => {
    console.log("Server started on port 5000");
})
