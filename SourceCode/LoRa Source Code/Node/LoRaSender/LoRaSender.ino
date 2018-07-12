#include <SPI.h>
#include <LoRa.h>

String inputString = "";
boolean stringComplete = false;

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

void setup()
{
  inputString.reserve(128); // allocate 128 bytes buffer in memory for manipulating Strings

  Serial.begin(9600);
  while (!Serial)
    ;

  LoRa.setPins(10, 8, 4);
  if (!LoRa.begin(868000000))
  {
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
  if (stringComplete)
  {
    if ((inputString.charAt(33) == '\r') && (inputString.charAt(34) == '\n'))
    { // Check if string is correct
      Serial.println(inputString);

      LoRa.beginPacket();
      LoRa.print(inputString);
      LoRa.endPacket();
    }

    inputString = "";
    stringComplete = false;
  }
}
