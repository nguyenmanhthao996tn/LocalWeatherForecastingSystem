const resultFolderPath = 'KetQua/';

var assert = require('assert');
var ObjectID = require('mongodb').ObjectID;
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://127.0.1.1:27017/WeatherForecast";

var fs = require('fs');

MongoClient.connect(url, function (err, db) {
    assert.equal(null, err);

    fs.readdir(resultFolderPath, function (err, items) {
        var recordCounter = 0;
        items.forEach(filename => {
            fs.readFile(resultFolderPath + filename, 'utf8', function (err, data) {
                if (err) throw err;

                // console.log('\n\n========== ' + filename + '==========');

                var recordArray = data.split('\n');
                recordArray.forEach(element => {
                    var recordElements = element.split('  ');

                    if (recordElements != '') {
                        if (recordElements.length == 3) {
                            var recordObject = {};

                            var dateString = '2018-' + filename.substring(0, 2) + '-' + filename.substring(2, 4) + ' ' + recordElements[0] + ':00:00';
                            recordObject.Date = new Date(dateString);
                            recordObject.AmountOfRain = recordElements[1];
                            recordObject.IsRain = (recordElements[2] != 0);
                            recordObject.nodeId = 'Weather_Node_01';

                            recordCounter++;
                            console.log(recordCounter);

                            // console.log('Record: ' + JSON.stringify(recordObject));
                            db.collection("ForecastResult").insertOne(recordObject, function (error, response) {
                                assert.equal(null, error);

                                // console.log('INSERTED');
                            });

                        } else {
                            console.log("Length: " + recordElements.length);
                        }
                    }
                });
            });
        });
    });
});