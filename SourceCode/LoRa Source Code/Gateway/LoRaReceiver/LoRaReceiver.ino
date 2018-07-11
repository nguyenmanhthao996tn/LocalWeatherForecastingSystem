#include <SPI.h>
#include "LoRa.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println("LoRa Receiver");

    if (!LoRa.begin(868000000))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    LoRa.setPins(15, 16, 5);

    LoRa.setSignalBandwidth(500000);
    LoRa.setCodingRate4(5);
    LoRa.setSpreadingFactor(12);
    LoRa.setPreambleLength(8);
    LoRa.setSyncWord(0x24);
}

void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available())
        {
            Serial.print((char)LoRa.read());
        }
        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }
}
