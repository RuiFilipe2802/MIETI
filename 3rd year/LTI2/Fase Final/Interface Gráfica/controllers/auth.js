const mysql = require("mysql");
const jwt = require("jsonwebtoken");
const bcrypt = require("bcryptjs");
const { promisify } = require("util");

const db = mysql.createConnection({
  host: process.env.DATABASE_HOST,
  user: process.env.DATABASE_USER,
  password: process.env.DATABASE_PASSWORD,
  database: process.env.DATABASE,
});

exports.login = async (req, res) => {
  try {
    const { email, password } = req.body;
    if (!email || !password) {
      return res.status(400).render("login", {
        message: "Please provide an email and password",
      });
    }
    db.query(
      "SELECT * FROM accounts WHERE email = ?",
      [email],
      async (error, resul) => {
        console.log(resul);
        if (!resul || !(await bcrypt.compare(password, resul[0].password))) {
          return res.status(400).render("login", {
            message: "Email or Password is incorrect",
          });
        } else {
          const id = resul[0].id;
          const token = jwt.sign({ id }, process.env.JWT_SECRET, {
            expiresIn: process.env.JWT_EXPIRES_IN,
          });
          console.log("The token is: " + token);
          const cookieOptions = {
            expires: new Date(
              Date.now() + process.env.JWT_COOKIE_EXPIRES * 24 * 60 * 60 * 1000
            ),
            httpOnly: true,
          };
          res.cookie("jwt", token, cookieOptions);
          if (email == "admin@gmail.com" && await bcrypt.compare(password, resul[0].password)) {
            res.status(200).redirect("/admin");
          } else {
            res.status(200).redirect("/dash");
          }
        }
      }
    );
  } catch (error) {
    console.log(error);
  }
};

exports.register = (req, res) => {
  console.log(req.body);
  const { name, email, password, passwordConfirm, idConcentrador } = req.body;
  db.query(
    "SELECT email FROM accounts WHERE email = ?",
    [email],
    async (error, results) => {
      if (error) {
        console.log(error);
      }
      if (results.length > 0) {
        return res.render("register", {
          message: "That email is already in use",
        });
      } else if (password !== passwordConfirm) {
        return res.render("register", {
          message: "Passwords do not match",
        });
      }
      let hashedPassword = await bcrypt.hash(password, 8);
      console.log(hashedPassword);
      db.query(
        "INSERT INTO accounts SET ?",
        { username: name, email: email, password: hashedPassword, idConcentrador: idConcentrador },
        (error, results) => {
          if (error) {
            console.log(error);
          } else {
            console.log(results);
            return res.render("register", {
              message: "User registered",
            });
          }
        }
      );
    }
  );
};

exports.isLoggedIn = async (req, res, next) => {
  // console.log(req.cookies);
  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          if (!result) {
            return next();
          }
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
          return next();
        }
      );
    } catch (error) {
      console.log(error);
      return next();
    }
  } else {
    next();
  }
};

exports.logout = async (req, res) => {
  res.cookie("jwt", "logout", {
    expires: new Date(Date.now() + 2 * 1000),
    httpOnly: true,
  });
  res.status(200).redirect("/");
};

exports.view = async (req, res, next) => {
  // console.log(req.cookies);
  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }

  let accounts = {};
  let concentradores = {};

  db.query(
    'SELECT * FROM accounts', (err, rows) => {
      // When done with the connection, release it
      if (!err) {
        accounts = rows;
      } else {
        console.log(err);
      }
    });

  db.query(
    'SELECT * FROM Concentrador', (err, rows) => {
      // When done with the connection, release it
      if (!err) {
        concentradores = rows;
        res.render('admin', { user: req.user, accounts, concentradores });
      } else {
        console.log(err);
      }
    });
};


exports.viewISS = async (req, res, next) => {
  // console.log(req.cookies);
  let idUser = 0;
  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      idUser = decoded.id;
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }

  
  db.query('SELECT * FROM Iss WHERE idConcentrador = (SELECT idConcentrador FROM accounts WHERE id = ?)',[idUser], (err, results) => {
    if (!err) {
      console.log("--------------");
      console.log(results);
      console.log("--------------");
      res.render("dash", { user: req.user, results });
    } else {
      console.log(err);
    }
  }
  );
};

exports.viewIdISS = (req, res) => {

  let luminosidade = {};
  let movimento = {};
  
  // User the connection
  db.query('SELECT * FROM AmostraLux WHERE idIss = ?',[req.params.id], (err, rows) => {
    // When done with the connection, release it
    if (!err) {
      luminosidade = rows;
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });

  db.query('SELECT * FROM AmostraMov WHERE idIss = ?',[req.params.id], (err, rows) => {
    // When done with the connection, release it
    if (!err) {
      movimento = rows;
      res.render('viewIss', { luminosidade, movimento });
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
}


exports.find = async (req, res, next) => {
  // console.log(req.cookies);
  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }
  let searchTerm = req.body.search;
  let searchTerm2 = req.body.search2;

  let accounts = {};
  let concentradores = {};

  db.query(
    'SELECT * FROM accounts WHERE username LIKE ?', ['%' + searchTerm + '%'], (err, rows) => {
      if (!err) {
        accounts = rows;
      } else {
        console.log(err);
      }
      console.log('The data from user table: \n', rows);
    });

  db.query(
    'SELECT * FROM Concentrador WHERE idConcentrador LIKE ?', ['%' + searchTerm2 + '%'], (err, rows) => {
      if (!err) {
        concentradores = rows;
        res.render('admin', { accounts, concentradores });
      } else {
        console.log(err);
      }
      console.log('The data from user table: \n', rows);
    });
};

exports.edit = async (req, res, next) => {

  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }

  db.query('SELECT * FROM accounts WHERE id = ?', [req.params.id], (err, rows) => {
    if (!err) {
      res.render('edit', { rows });
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
};


exports.editConcentradores = async (req, res, next) => {

  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM Concentrador WHERE idConcentrador = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }

  db.query('SELECT * FROM Concentrador WHERE idConcentrador = ?', [req.params.id], (err, rows) => {
    if (!err) {
      res.render('editConcentradores', { rows });
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
};

exports.update = async (req, res, next) => {

  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }
  const { username, email, idConcentrador } = req.body;


  db.query('UPDATE accounts SET username = ?, email = ?, idConcentrador = ? WHERE id = ?', [username, email, idConcentrador, req.params.id], (err, rows) => {

    if (!err) {
      res.status(200).redirect("/admin");
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
};

exports.updateConcentradores = async (req, res, next) => {

  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }
  const { idConcentrador, x, y } = req.body;


  db.query('UPDATE Concentrador SET idConcentrador = ?, x = ?, y = ? WHERE idConcentrador = ?', [idConcentrador, x, y, req.params.id], (err, rows) => {

    if (!err) {
      res.status(200).redirect("/admin");
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
};


exports.delete = async (req, res, next) => {
  // console.log(req.cookies);
  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }
  db.query('DELETE FROM accounts WHERE id = ?', [req.params.id], (err, rows) => {
    if (!err) {
      res.status(200).redirect("/admin");
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
};


exports.deleteConcentradores = async (req, res, next) => {
  // console.log(req.cookies);
  if (req.cookies.jwt) {
    try {
      //1) verify the token
      const decoded = await promisify(jwt.verify)(
        req.cookies.jwt,
        process.env.JWT_SECRET
      );
      console.log(decoded);
      //2) Check if the user still exists
      db.query(
        "SELECT * FROM accounts WHERE id = ?",
        [decoded.id],
        (error, result) => {
          console.log(result);
          req.user = result[0];
          console.log("user is");
          console.log(req.user);
        }
      );
    } catch (error) {
      console.log(error);
    }
  }

  db.query('DELETE FROM Concentrador WHERE idConcentrador = ?', [req.params.id], (err, rows) => {
    console.log('--------------------\n----------------');
    console.log(req.params.id);
    if (!err) {
      res.status(200).redirect("/admin");
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
};

exports.form = (req, res) => {
  res.render('addConcentrador');
}


exports.addConcentrador = (req, res) => {
  const { idConcentrador, x, y } = req.body;

  // User the connection
  db.query('INSERT INTO Concentrador SET idConcentrador = ?, x = ?, y = ?', [idConcentrador, x, y], (err, rows) => {
    if (!err) {
      res.status(200).redirect("/admin");
    } else {
      console.log(err);
    }
    console.log('The data from user table: \n', rows);
  });
}