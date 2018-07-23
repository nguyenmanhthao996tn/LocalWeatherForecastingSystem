#include <SPI.h>
#include <LoRa.h>

String inputString = "";
boolean stringComplete = false;
unsigned long now, lastCounter;

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
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  inputString.reserve(128); // allocate 128 bytes buffer in memory for manipulating Strings

  Serial.begin(9600);
  while (!Serial)
    ;

  LoRa.setPins(10, 8, 2);
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
  now = millis();

  if (stringComplete)
  {
    if ((inputString.charAt(33) == '\r') && (inputString.charAt(34) == '\n') && (now - lastCounter > 5000))
    { // Check if string is correct
      lastCounter = now;

      LoRa.beginPacket();
      LoRa.print(inputString.substring(0, 33));
      LoRa.endPacket();

      Serial.println(inputString.substring(0, 33));
    }

    inputString = "";
    inputString.setCharAt(33, '\0');
    inputString.setCharAt(34, '\0');
    stringComplete = false;
  }
}
