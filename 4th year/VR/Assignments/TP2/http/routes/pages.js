const express = require('express');
const authController = require('../controllers/auth');

const router = express.Router();

router.get('/', authController.isLoggedIn, (req, res) => {
    if (req.user) {
        res.render('profile', {
            user: req.user
        });
    } else {
        res.redirect('http://localhost:5000/login');
    }
})

router.get('/admin',authController.admin,(req, res) => {
    //console.log(req.user)
    //console.log("OLAAAA")
    //console.log(req.user[0])
    if(req.user){
        res.render('admin',{
            user : req.user
        })
    }else {
        res.redirect('http://localhost:5000/login');
    }
})

router.get('/profile', authController.isLoggedIn, (req, res) => {
    console.log(req.user)
    if (req.user.role == 'user') {
        res.render('profile', {
            user:
                { id: req.user.id, name: req.user.name, email: req.user.email },

        });
    } else if (req.user.role == 'admin') {
        console.log("AQUI CRL")
        res.render('admin', {
            user:
                { id: req.user.id, name: req.user.name, email: req.user.email },

        });
    } else {
        res.redirect('http://localhost:5000/login');
    }

})

module.exports = router