#include <SPI.h>
#include <LoRa.h>

int counter = 0;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println("LoRa Sender");

    if (!LoRa.begin(868000000))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }

    LoRa.setPins(10, 8, 4);

    LoRa.setSignalBandwidth(500000);
    LoRa.setCodingRate4(5);
    LoRa.setSpreadingFactor(12);
    LoRa.setPreambleLength(8);
    LoRa.setSyncWord(0x24);
}

void loop()
{
    Serial.print("Sending packet: ");
    Serial.println(counter);

    // send packet
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;
    delay(2000);
}
