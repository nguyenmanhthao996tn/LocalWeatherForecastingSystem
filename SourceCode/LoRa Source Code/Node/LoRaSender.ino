#include <SPI.h>
#include <LoRa.h>
#define LED 9
int counter = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(868000000)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSignalBandwidth(500000);
  LoRa.setCodingRate4(5);
  LoRa.setSpreadingFactor(12); 
  LoRa.setPreambleLength(8);
  //LoRa.setSyncWord(0x14);

  pinMode(LED,OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  digitalWrite(LED, LOW);
  delay(2000);
}
