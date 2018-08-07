const SERVER_NAME = "127.0.0.1";

var mosca = require('mosca');
var net = require('net');

var ascoltatore = {
  //using ascoltatore
  type: 'mongo',
  url: 'mongodb://'+ SERVER_NAME +':27017/mqtt',
  pubsubCollection: 'ascoltatori',
  mongo: {}
};

var settings = {
  port: 5002,
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

  if ((str.length == 35) && (str[33] == '\r') && (str[34] == '\n')) { 
    var client = new net.Socket();

    client.on('data', function (data) {
      consoleLogWithISODate('Received: ' + data);
      client.destroy(); // kill client after server's response
    });

    client.on('close', function () {
      consoleLogWithISODate('Connection closed');
    });

    client.connect(5001, SERVER_NAME, function () {
      consoleLogWithISODate('Connected');

      var weatherDataWriteRequestObject = { code: 1 };
      weatherDataWriteRequestObject.data = packet.payload.toString();
      client.write(JSON.stringify(weatherDataWriteRequestObject));
    });
  }
});

server.on('ready', setup);

// fired when the mqtt server is ready
function setup() {
  consoleLogWithISODate('Mosca server is up and running');
}

function consoleLogWithISODate(str) {
  var d = new Date();
  var n = d.toISOString();

  console.log(n + ': ' + str);
}
