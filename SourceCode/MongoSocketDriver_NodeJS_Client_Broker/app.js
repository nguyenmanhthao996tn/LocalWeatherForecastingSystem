const MONGO_SERVER_NAME = "127.0.0.1";
const MOSCA_PORT = 5002;

var mosca = require('mosca');
var net = require('net');

var ascoltatore = {
  //using ascoltatore
  type: 'mongo',
  url: 'mongodb://' + MONGO_SERVER_NAME + ':27017/mqtt',
  pubsubCollection: 'ascoltatori',
  mongo: {}
};

var settings = {
  port: MOSCA_PORT,
  backend: ascoltatore
};

var server = new mosca.Server(settings);

server.on('clientConnected', function (client) {
  consoleLogWithISODate('client connected', client.id);
});

// fired when a message is received
server.on('published', function (packet, client) {
  var inputString = packet.payload.toString();
  consoleLogWithISODate('Publish ' + inputString + ' | Length: ' + inputString.length);

  var inputSplitedString = inputString.split(',');
  var stackedValue = [];
  for (var i = 0; i < inputSplitedString.length; i++) {
    var temp = parseInt(inputSplitedString[i]);
    stackedValue.push(temp);
  }

  if ((stackedValue.length == 17) && (stackedValue[15] == 13) && (stackedValue[16] == 10)) {
    var client = new net.Socket();

    client.on('data', function (data) {
      consoleLogWithISODate('Received: ' + data);
      client.destroy(); // kill client after server's response
    });

    client.on('close', function () {
      consoleLogWithISODate('Connection closed');
    });

    client.connect(5001, MONGO_SERVER_NAME, function () {
      consoleLogWithISODate('Connected');

      var weatherDataWriteRequestObject = getWriteRequestObject(stackedValue);
      // weatherDataWriteRequestObject.nodeId = packet.payload.toString().substring(0, 15);
      // weatherDataWriteRequestObject.data = packet.payload.toString().substring(15);
      client.write(JSON.stringify(weatherDataWriteRequestObject));
    });
  }
});

server.on('ready', setup);

// fired when the mqtt server is ready
function setup() {
  consoleLogWithISODate('Mosca server is up and running on port: ' + MOSCA_PORT);
}

function consoleLogWithISODate(str) {
  var d = new Date();
  var n = d.toISOString();

  console.log(n + ': ' + str);
}

function getWriteRequestObject(stackedValue) {
  var weatherDataWriteRequestObject = { code: 1, nodeId: '', data: '' };
  var nodeIdHighByte = stackedValue[0];
  var nodeIdLowByte = stackedValue[1];

  weatherDataWriteRequestObject.nodeId = ((nodeIdHighByte << 8) | nodeIdLowByte).toString();

  var buffer = "";
  for (var i = 0; i < stackedValue.length - 3; i++) {
    buffer += stackedValue[i].toString() + ",";
  }
  buffer += stackedValue[stackedValue.length - 3];
  weatherDataWriteRequestObject.data = buffer;

  return weatherDataWriteRequestObject;
}

