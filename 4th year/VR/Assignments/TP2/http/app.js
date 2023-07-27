const express = require("express");
const mysql = require("mysql");
const dotenv = require('dotenv');
const path = require('path');
const cookieParser = require('cookie-parser');

dotenv.config({ path: './.env'})

const app = express();

const publicDirectory = path.join(__dirname,'./public');
app.use(express.static(publicDirectory));

//parse URL encoded bodies (as sent by HTML forms)
app.use(express.urlencoded({ extended: false }))
//parse JSON bodies
app.use(express.json());

app.use(cookieParser());

app.set('view engine', 'hbs');

//Define Routes
app.use('/',require('./routes/pages'));
app.use('/auth', require('./routes/auth'));

app.listen(5001, ()=>{
    console.log("Server started on port 5001");
})
