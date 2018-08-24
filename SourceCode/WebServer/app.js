var path = require('path');
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

var assert = require('assert');
var ObjectID = require('mongodb').ObjectID;
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://127.0.1.1:27017/WeatherForecast";

app.use('/static', express.static(path.join(__dirname, 'web_sources/')));

app.get('/', function (req, res) {
  res.sendfile('web_sources/weather_login.html', function (err) {
    if (err) {
      console.log(err);
    }
    else {
      console.log('Sent: weather_login.html');
    }
  });
});

io.on('connection', function (socket) {
  console.log('IO: a user connected');

  socket.on('type', function (msg) {
    switch (msg) {
      case 'LOGIN':
        console.log('IO: Login');
        socket.on('login-info', function (msg) {
          console.log('IO: ' + msg.username + "|" + msg.password);

          // generate seasion-key
          checkAccountAvailable(msg.username, msg.password, (seasionKeyObject) => {
            socket.emit('login-request', seasionKeyObject);
          });
        });
        break;
      case 'INDEX':
        console.log('IO: Index ');

        socket.on('seasion-info', function (info) {
          var username = info.username;
          var seasionKey = info.seasion;
          console.log('seasion-info: ' + username + ' | ' + seasionKey);

          // Check seasion key
          MongoClient.connect(url, function (err, db) {
            assert.equal(null, err);

            // Check account
            var querryObject = { "username": username };
            var fieldSelect = { _id: 0, 'password': 0, 'role': 0, 'seasionKey': 0 };
            db.collection("Users").findOne(querryObject, fieldSelect, function (err, result) {
              assert.equal(null, err);

              var dataObject = {};
              if (result != null) {
                dataObject.seasionKeyStatus = 'OK';

                // Get data of user
                dataObject.userData = result;

                // First node data
                var firstNodeId = result.userData.owningNode[0];
                if (firstNodeId) {
                  
                }
              } else {
                dataObject.seasionKeyStatus = 'Expired';
              }

              // Send data
              socket.emit('index_data', dataObject);
            });
          });
        });
        socket.emit('seasion-info', 'request');
        break;
      default:
        break;
    }
  });
});

http.listen(8080, () => {
  console.log('listening on *:8080');
});

function checkAccountAvailable(username, password, callback) {
  var resultObject = {};

  MongoClient.connect(url, function (err, db) {
    assert.equal(null, err);

    // Check account
    var querryObject = { "username": username, "password": password };
    db.collection("Users").findOne(querryObject, function (err, result) {
      assert.equal(null, err);

      console.log('LOGIN: ' + result);
      if (result != null) {
        resultObject.accountAvailability = true;

        // Generate Seasion key
        var str = "";
        for (; str.length < 32; str += Math.random().toString(36).substr(2));
        resultObject.seasionKey = str.substr(0, 32);

        // Write seasion key to database
        var expireDate = new Date();
        expireDate.setDate(expireDate.getDate() + 1);
        var updateValue = {
          $set: {
            'seasionKey': {
              'key': resultObject.seasionKey,
              'expire': expireDate
            }
          }
        };

        db.collection("Users").updateOne(querryObject, updateValue, function (err, res) {
          assert.equal(null, err);

          console.log("MONGO: 1 document updated");
          db.close();
        });

      } else {
        resultObject.accountAvailability = false;
        db.close();
      }

      callback(resultObject);
    });
  });
}
