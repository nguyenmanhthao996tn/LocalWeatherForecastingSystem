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

        var g_username = '';

        socket.on('sign-out', () => {
          var querryObject = { "username": g_username };
          var updateObject = { '$set': { "seasionKey": {} } };

          MongoClient.connect(url, function (err, db) {
            assert.equal(null, err);

            db.collection("Users").updateOne(querryObject, updateObject, function (err, result) {
              assert.equal(null, err);
              console.log(g_username + " loged out");
              db.close();
            });
          });
        });

        socket.on('seasion-info', function (info) {
          var username = info.username;
          var seasionKey = info.seasion;
          console.log('seasion-info: ' + username + ' | ' + seasionKey);

          // Check seasion key
          MongoClient.connect(url, function (err, db) {
            assert.equal(null, err);

            // Check account
            var querryObject = { "username": username };
            var fieldSelect = { _id: 0, 'password': 0, 'role': 0 };
            db.collection("Users").findOne(querryObject, fieldSelect, function (err, result) {
              assert.equal(null, err);

              var dataObject = {};
              if ((result != null) && (result.seasionKey.key == seasionKey)) {
                dataObject.seasionKeyStatus = 'OK';
                g_username = username;

                // Get data of user
                dataObject.userInfo = result;

                // First node data
                var firstNodeId = result.owningNode[0];
                if (firstNodeId) {
                  var querryObject = { "nodeId": firstNodeId };
                  db.collection("Nodes").findOne(querryObject, { '_id': 0 }, function (err, result) {
                    assert.equal(null, err);

                    dataObject.firstNode = {};
                    dataObject.firstNode.info = result;

                    var querryObject = { "nodeId": firstNodeId };
                    var filterObject = { '_id': 0 };
                    db.collection("WeatherData").find(querryObject, filterObject).sort({ 'Time': -1 }).limit(100).toArray(function (err, result) {
                      assert.equal(null, err);

                      dataObject.firstNode.data = {};
                      dataObject.firstNode.data.sensorData = formatSensorDataForWebTable(result);

                      var querryObject = { "nodeId": firstNodeId };
                      var filterObject = { '_id': 0 };
                      db.collection("ForecastResult").find(querryObject, filterObject).sort({ 'Time': -1 }).limit(100).toArray(function (err, result) {
                        assert.equal(null, err);

                        dataObject.firstNode.data.forecastResult = formatForecastResultForWebTable(result);

                        // Send data
                        socket.emit('index_data', dataObject);
                      });
                    });
                  });
                }
              } else {
                dataObject.seasionKeyStatus = 'Expired';

                // Send data
                socket.emit('index_data', dataObject);
              }
            });
          });
        });

        socket.on('request-node-data', (dataObject) => {
          // console.log(JSON.stringify(dataObject));

          var username = dataObject.username;
          var seasionKey = dataObject.seasion;
          var nodeId = dataObject.node_id;

          // Check username & seasion key
          MongoClient.connect(url, function (err, db) {
            assert.equal(null, err);

            // Check account
            var querryObject = { "username": username };
            var fieldSelect = { _id: 0, 'seasionKey': 1 };
            db.collection("Users").findOne(querryObject, fieldSelect, function (err, result) {
              assert.equal(null, err);

              if ((result != null) && (result.seasionKey.key == seasionKey)) {
                var querryObject = { "nodeId": nodeId };
                var filterObject = { '_id': 0 };

                var responseObject = { info: {}, sensorData: [], forecastResult: [] };

                db.collection("Nodes").findOne(querryObject, { '_id': 0 }, function (err, result) {
                  assert.equal(null, err);

                  responseObject.info = result;

                  db.collection("WeatherData").find(querryObject, filterObject).sort({ 'Time': -1 }).limit(100).toArray(function (err, result) {
                    assert.equal(null, err);

                    responseObject.sensorData = formatSensorDataForWebTable(result);

                    db.collection("ForecastResult").find(querryObject, filterObject).sort({ 'Time': -1 }).limit(100).toArray(function (err, result) {
                      assert.equal(null, err);

                      responseObject.forecastResult = formatForecastResultForWebTable(result);

                      socket.emit('response-node-data', responseObject);
                    });
                  });
                });
              }
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

function formatSensorDataForWebTable(data) {
  var resultObject = [];

  for (var i = 0; i < data.length; i++) {
    var row = [];
    var object = data[i];

    row.push((i + 1).toString());
    row.push((new Date(object.Time)).toLocaleString());
    row.push(object.AirDirection);
    row.push(object.AirSpeed1Min);
    row.push(object.AirSpeed5Min);
    row.push(object.Temperature);
    row.push(object.Humidity);
    row.push(object.Atmosphere);
    row.push(object.Rainfall1Hour);
    row.push(object.Rainfall24Hour);

    resultObject.push(row);
  }

  return resultObject;
}

function formatForecastResultForWebTable(data) {
  var resultObject = [];

  for (var i = 0; i < data.length; i++) {
    var row = [];
    var object = data[i];

    row.push((i + 1).toString());
    row.push((new Date(object.Date)).toLocaleString());
    row.push(object.AmountOfRain);

    resultObject.push(row);
  }

  return resultObject;
}
