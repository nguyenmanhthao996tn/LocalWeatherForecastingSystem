const SERVER_NAME = "127.0.1.1";

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
  console.log('client connected', client.id);
});

// fired when a message is received
server.on('published', function (packet, client) {
  console.log('Publish ', packet.payload.toString());
  var client = new net.Socket();

  client.on('data', function (data) {
    console.log('Received: ' + data);
    client.destroy(); // kill client after server's response
  });

  client.on('close', function () {
    console.log('Connection closed');
  });

  client.connect(5001, SERVER_NAME, function () {
    console.log('Connected');

    var weatherDataWriteRequestObject = { code: 1 };
    weatherDataWriteRequestObject.data = packet.payload.toString();
    client.write(JSON.stringify(weatherDataWriteRequestObject));
  });
});

server.on('ready', setup);

// fired when the mqtt server is ready
function setup() {
  console.log('Mosca server is up and running');
}
