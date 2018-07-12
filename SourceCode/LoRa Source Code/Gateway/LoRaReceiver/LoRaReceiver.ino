#include <SPI.h>
#include "LoRa.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("LoRa Receiver");

  LoRa.setPins(15, 16, 5);
  if (!LoRa.begin(868000000))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  LoRa.setSignalBandwidth(500000);
  LoRa.setCodingRate4(5);
  LoRa.setSpreadingFactor(12);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0x24);
}

void loop()
{
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
      Serial.print(packetSize);
      Serial.print(" Received packet '");
  
      while (LoRa.available())
      {
        Serial.print((char)LoRa.read());
      }

      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }
}
