const SERVER_NAME = "127.0.1.1";

var mosca = require('mosca');
var net = require('net');
var schedule = require('node-schedule');

var ascoltatore = {
  //using ascoltatore
  type: 'mongo',
  url: 'mongodb://' + SERVER_NAME + ':27017/mqtt',
  pubsubCollection: 'ascoltatori',
  mongo: {}
};

var settings = {
  port: 5002,
  backend: ascoltatore
};

var server = new mosca.Server(settings);
var dataObjectArray = [];

server.on('clientConnected', function (client) {
  console.log('client connected', client.id);
});
server.on('published', function (packet, client) {
  var str = packet.payload.toString();

  if ((str.length == 37) && (str[33] == '\\') && (str[34] == 'r') && (str[35] == '\\') && (str[36] == 'n')) {
    console.log('Add ', str);
    dataObjectArray.push(dataStringParse(packet.payload.toString()));
  }
});
server.on('ready', setup);

// Start scheduler
var scheduler = schedule.scheduleJob('*/5 * * * *', function () {
  if (dataObjectArray.length > 0) {
    var socketClient = new net.Socket();
    socketClient.on('data', function (data) {
      console.log('Received: ' + data);
      socketClient.destroy(); // kill client after server's response
    });
    socketClient.on('close', function () {
      console.log('Connection closed');
    });
    socketClient.connect(5001, SERVER_NAME, function () {
      console.log('Connected');
      var weatherDataWriteRequestObject = { code: 1 };
      weatherDataWriteRequestObject.data = dataObjectArrayToString(dataObjectArray);
      dataObjectArray = [];
      socketClient.write(JSON.stringify(weatherDataWriteRequestObject));
    });
  }
});

// fired when the mqtt server is ready
function setup() {
  console.log('Mosca server is up and running');
}

function dataStringParse(dataString) {
  var splittedStringArray = dataString.split(/c|s|g|t|r|p|h|b|\r|\n/);

  var result = {};
  result.airDirection = parseInt(splittedStringArray[1]) || 0;
  result.airSpeed1Min = parseInt(splittedStringArray[2]) || 0;
  result.airSpeed5Min = parseInt(splittedStringArray[3]) || 0;
  result.temperature = parseInt(splittedStringArray[4]) || 0;
  result.rainfall1Hour = parseInt(splittedStringArray[5]) || 0;
  result.rainfall24Hour = parseInt(splittedStringArray[6]) || 0;
  result.humidity = parseInt(splittedStringArray[7]) || 0;
  result.atmosphere = parseInt(splittedStringArray[8]) || 0;

  return result;
}

function dataObjectArrayToString(dataObjectArray) {
  var outputAirDirectionValue = [0, 45, 90, 135, 180, 225, 270, 315];
  var airDirectionCounterArray = [0, 0, 0, 0, 0, 0, 0, 0];
  var airSpeed1Min = 0;
  var airSpeed5Min = 0;
  var temperature = 0;
  var rainfall1Hour = 0;
  var rainfall24Hour = 0;
  var humidity = 0;
  var atmosphere = 0;

  for (var i = 0; i < dataObjectArray.length; i++) {
    switch (dataObjectArray[i].airDirection) {
      case 0:
        airDirectionCounterArray[0]++;
        break;
      case 45:
        airDirectionCounterArray[1]++;
        break;
      case 90:
        airDirectionCounterArray[2]++;
        break;
      case 135:
        airDirectionCounterArray[3]++;
        break;
      case 180:
        airDirectionCounterArray[4]++;
        break;
      case 225:
        airDirectionCounterArray[5]++;
        break;
      case 270:
        airDirectionCounterArray[6]++;
        break;
      case 315:
        airDirectionCounterArray[7]++;
        break;
      default:
        break;
    }

    // airSpeed1Min
    airSpeed1Min += dataObjectArray[i].airSpeed1Min;

    // airSpeed5Min
    airSpeed5Min += dataObjectArray[i].airSpeed5Min;

    // temperature
    temperature += dataObjectArray[i].temperature;

    // rainfall1Hour
    rainfall1Hour += dataObjectArray[i].rainfall1Hour;

    // rainfall24Hour
    rainfall24Hour += dataObjectArray[i].rainfall24Hour;

    // humidity
    humidity += dataObjectArray[i].humidity;

    // atmosphere
    atmosphere += dataObjectArray[i].atmosphere;
  }

  airDirection = outputAirDirectionValue[airDirectionCounterArray.indexOf(Math.max(...airDirectionCounterArray))];
  airSpeed1Min /= dataObjectArray.length;
  airSpeed5Min /= dataObjectArray.length;
  temperature /= dataObjectArray.length;
  rainfall1Hour /= dataObjectArray.length;
  rainfall24Hour /= dataObjectArray.length;
  humidity /= dataObjectArray.length;
  atmosphere /= dataObjectArray.length;

  var output = 'c' + airDirection + 's' + airSpeed1Min.toFixed(0) + 'g' + airSpeed5Min.toFixed(0) + 't' + temperature.toFixed(0) + 'r' + rainfall1Hour.toFixed(0) + 'p' + rainfall24Hour.toFixed(0) + 'h' + humidity.toFixed(0) + 'b' + atmosphere.toFixed(0) + '\r\n';
  return output;
}
