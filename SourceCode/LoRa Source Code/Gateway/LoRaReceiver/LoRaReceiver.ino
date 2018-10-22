/*********** Libraries ***********/
#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "LcdController.h"

/*********** Constants ***********/
#define ssid "UIT Public"
#define password ""
//#define ssid "ThaoNguyen-PC"
//#define password "a123456789"

#define client_id "Node_1"
#define mqtt_server "10.71.8.10"
#define mqtt_topic_pub "topic/SensorData"
#define mqtt_topic_sub "topic/ControlData"
const uint16_t mqtt_port = 5002;

#define receiveBufferSize 128
#define sendingBufferSize 1024

/*********** Global variables ***********/
WiFiClient espClient;
PubSubClient client(espClient);
char receiveBuffer[receiveBufferSize];
char sendingBuffer[sendingBufferSize];

/*********** Method headers ***********/
void setupLoRa(void);
void setupWifi(void);
void reconnectToWifi(void);
void mqttClientOnMessagecallback(char *topic, byte *payload, unsigned int length);

/*********** Methods define ***********/
void setup()
{
  memset(receiveBuffer, 0, receiveBufferSize);
  memset(sendingBuffer, 0, sendingBufferSize);

  Serial.begin(115200);
  Serial.println("LoRa Receiver");

  setupLoRa();
  setupWifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttClientOnMessagecallback);
}

void loop()
{
  char sendingBuffer_temp[128];

  if (!client.connected())
  {
    reconnectToWifi();
  }

  client.loop();

  int packetSize = LoRa.parsePacket();
  if (packetSize > 0)
  {
    Serial.print(packetSize);
    Serial.print(" Received packet '");

    memset(receiveBuffer, 0, receiveBufferSize);
    memset(sendingBuffer, 0, sendingBufferSize);

    for (int i = 0; i < packetSize - 1; i++)
    {
      receiveBuffer[i] = (char)LoRa.read();
      snprintf(sendingBuffer_temp, 128, "%d,", receiveBuffer[i]);
      strcat(sendingBuffer, sendingBuffer_temp);
    }

    // receiveBuffer[packetSize - 1]
    receiveBuffer[packetSize - 1] = (char)LoRa.read();
    snprintf(sendingBuffer_temp, 128, "%d", receiveBuffer[packetSize - 1]);
    strcat(sendingBuffer, sendingBuffer_temp);

    receiveBuffer[packetSize] = '\0';

    Serial.print(sendingBuffer);

    // Publish to MQTT
    client.publish(mqtt_topic_pub, sendingBuffer);

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  else if (packetSize == -1)
  {
    // Publish to MQTT
    client.publish(mqtt_topic_pub, "CRC ERROR");
  }
}

void setupLoRa(void)
{
  LoRa.setPins(15, 16, 5);
  if (!LoRa.begin(868300000))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  LoRa.setSignalBandwidth(125000);
  LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(12);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0x52);
  LoRa.enableCrc();
}

void setupWifi(void)
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectToWifi(void)
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(client_id))
    {
      Serial.println("connected");
      client.subscribe(mqtt_topic_sub);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttClientOnMessagecallback(char *topic, byte *payload, unsigned int length)
{
  // Handle control message received from server
  char buffer[25];

  LcdController.getResultScreenString(buffer, 25, 0);
}
