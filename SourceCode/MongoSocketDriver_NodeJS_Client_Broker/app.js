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
  var str = packet.payload.toString();
  consoleLogWithISODate('Publish ' + str + ' | Length: ' + str.length);

  if ((str.length == 17) && (str[15] == '\r') && (str[16] == '\n')) {
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

      var weatherDataWriteRequestObject = getWriteRequestObject(packet.payload.toString());
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

function getWriteRequestObject(payloadString) {
  var weatherDataWriteRequestObject = { code: 1, nodeId: '', data: '' };
  var nodeIdHighByte = payloadString[0];
  var nodeIdLowByte = payloadString[1];
  
  weatherDataWriteRequestObject.nodeId = (nodeIdHighByte << 8) | nodeIdLowByte;
  weatherDataWriteRequestObject.data = payloadString;

  return weatherDataWriteRequestObject;
}
