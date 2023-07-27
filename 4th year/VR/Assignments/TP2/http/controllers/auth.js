const jwt = require('jsonwebtoken');
const { promisify } = require('util');
const csv = require("csvtojson");


exports.isLoggedIn = async (req, res, next) => {
    console.log(req.cookies);

    if (req.cookies.jwt) {
        try {
            const decoded = await promisify(jwt.verify)(req.cookies.jwt, process.env.JWT_SECRET_ADMIN);
            //console.log(decoded);
            var user =
                { id: decoded.id, name: decoded.name, email: decoded.email, password: decoded.password, role: decoded.role };
            console.log(user)
            req.user = user
            return next();
        } catch (error) {
            console.log(error)
        }
        try {
            const decoded = await promisify(jwt.verify)(req.cookies.jwt, process.env.JWT_SECRET);
            //console.log(decoded.role);
            const user =
                { id: decoded.id, name: decoded.name, email: decoded.email, password: decoded.password, role: decoded.role };
            //console.log(user)
            var today = new Date();
            var date = today.getFullYear() + '-' + (today.getMonth() + 1) + '-' + today.getDate();
            var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
            var dateTime = date + ' ' + time;
            const fs = require('fs');
            // write JSON string to a file
            fs.appendFile('users.csv', user.id + "," + user.name + "," + user.email + "," + dateTime + '\n', (err) => {
                if (err) {
                    throw err;
                }
                console.log("JSON data is saved.");
            });
            req.user = user
            return next();
        } catch (error) {
            console.log(error)
            return next();
        }
    } else {
        next();
    }
}
exports.logout = async (req, res, next) => {
    res.cookie('jwt', 'logout', {
        expires: new Date(Date.now() + 2 * 1000),
        httpOnly: true
    });
    res.status(200).redirect('http://localhost:5000/login');
}

exports.admin = (req, res, next) => {
    const CSVToJSON = require('csvtojson');

    // convert users.csv file to JSON array
    CSVToJSON().fromFile('users.csv')
        .then(users => {

            // users is a JSON array
            // log the JSON array
            //console.log(users);
            req.user = users
            return next();
        }).catch(err => {
            // log error if any
            console.log(err);
        });
}
