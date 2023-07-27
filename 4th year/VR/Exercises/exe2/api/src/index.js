const express = require('express');
const mysql = require('mysql');

const app = express();

const connection = mysql.createConnection({
    host: '172.17.0.2',
    user: 'root',
    password: '12345',
    database: 'alunos'
});

connection.connect();

app.get ('/identificacao', function(req, res){
    connection.query('SELECT * FROM identificacao', function(error, results){
        if(error){
            throw error
        };

        res.send(results.map(item => ({name: item.name, number: item.number, email: item.email} 
            )));
    });
});

app.listen(9001,'0.0.0.0', function(){
    console.log('Listening on port 9001')
})