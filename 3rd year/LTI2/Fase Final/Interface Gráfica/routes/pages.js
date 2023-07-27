const express = require("express");
const authController = require("../controllers/auth");
const mysql = require("mysql");

const router = express.Router();

const db = mysql.createConnection({
  host: process.env.DATABASE_HOST,
  user: process.env.DATABASE_USER,
  password: process.env.DATABASE_PASSWORD,
  database: process.env.DATABASE,
});

router.get("/", authController.isLoggedIn, (req, res) => {
  res.render("index", {
    user: req.user,
  });
});


router.get("/register", (req, res) => {
  res.render("register");
});

router.get("/login", (req, res) => {
  res.render("login");
});

router.get("/profile", authController.isLoggedIn, (req, res) => {
  console.log(req.user);
  if (req.user) {
    res.render("profile", {
      user: req.user,
    });
  } else {
    res.redirect("/login");
  }
});


router.get('/admin',authController.view);
router.get('/dash', authController.viewISS);
router.post('/admin',authController.find);
router.get('/edit/:id',authController.edit);
router.post('/edit/:id',authController.update);
router.get('/dash/:id', authController.viewIdISS);
router.get('/addConcentrador', authController.form);
router.post('/addConcentrador',authController.addConcentrador);
router.get('/editConcentradores/:id',authController.editConcentradores);
router.post('/editConcentradores/:id',authController.updateConcentradores);
router.get('/:id',authController.delete);
router.get('/remove/:id',authController.deleteConcentradores);

module.exports = router;
