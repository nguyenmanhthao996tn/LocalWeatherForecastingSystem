#include <SPI.h>
#include <LoRa.h>
#include "OutputData.h"

char sendingDataStringBuffer[255];
String inputString = "";
boolean stringComplete = false;
unsigned long now, lastSenderCounter, lastGetDataCounter;

const uint16_t windDirectionValueArray[] = {0, 45, 90, 135, 180, 225, 270, 315};
uint16_t windDirectionCounterArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
stcOutputData_t *averageDataObject, *newDataObject;
uint16_t objectCounter;

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

uint16_t addToAverage(uint16_t averageValue, uint16_t counter, uint16_t newValue)
{
  uint32_t tempValue = averageValue * counter + newValue;
  return (averageValue / (counter + 1));
}

uint16_t getMostAppearValue(const uint16_t valueArray[], uint16_t counterArray[], uint16_t size)
{
  uint16_t maxCounter = counterArray[0];
  uint16_t i;
  for (i = 1; i < size; i++)
  {
    if (maxCounter < counterArray[i])
    {
      maxCounter = counterArray[i];
    }
  }

  return valueArray[maxCounter];
}

void addNewObjectToAverageObject(stcOutputData_t *averageDataObject, stcOutputData_t *newDataObject)
{
  // airSpeed1Min
  averageDataObject->airSpeed1Min = addToAverage(averageDataObject->airSpeed1Min, objectCounter, (newDataObject->airSpeed1Min));

  // airSpeed5Min
  averageDataObject->airSpeed5Min = addToAverage(averageDataObject->airSpeed5Min, objectCounter, (newDataObject->airSpeed5Min));

  // temperature
  averageDataObject->temperature = addToAverage(averageDataObject->temperature, objectCounter, (newDataObject->temperature));

  // rainfall1Hour
  averageDataObject->rainfall1Hour = addToAverage(averageDataObject->rainfall1Hour, objectCounter, (newDataObject->rainfall1Hour));

  // rainfall24Hour
  averageDataObject->rainfall24Hour = addToAverage(averageDataObject->rainfall24Hour, objectCounter, (newDataObject->rainfall24Hour));

  // humidity
  averageDataObject->humidity = addToAverage(averageDataObject->humidity, objectCounter, (newDataObject->humidity));

  // atmosphere
  averageDataObject->atmosphere = addToAverage(averageDataObject->atmosphere, objectCounter, (newDataObject->atmosphere));

  objectCounter++;
}

void setup()
{
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  objectCounter = 0;
  lastGetDataCounter = 0;
  lastSenderCounter = 0;
  stcOutputData_Create(&averageDataObject);

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
    // Check counter to get data every 5 seconds
    if ((inputString.charAt(33) == '\r') && (inputString.charAt(34) == '\n') && (now - lastGetDataCounter > 5000))
    { // Check if string is correct
      lastGetDataCounter = now;
      inputString.setCharAt(35, '\0');

      // Create and parse new object
      stcOutputData_Parse(inputString.c_str(), &newDataObject);

      // Add values from new object to average object
      addNewObjectToAverageObject(averageDataObject, newDataObject);
      switch (newDataObject->airDirection)
      {
      case 0:
        windDirectionCounterArray[0]++;
        break;
      case 45:
        windDirectionCounterArray[1]++;
        break;
      case 90:
        windDirectionCounterArray[2]++;
        break;
      case 135:
        windDirectionCounterArray[3]++;
        break;
      case 180:
        windDirectionCounterArray[4]++;
        break;
      case 225:
        windDirectionCounterArray[5]++;
        break;
      case 270:
        windDirectionCounterArray[6]++;
        break;
      case 315:
        windDirectionCounterArray[7]++;
        break;
      default:
        break;
      }

      // Delete new object
      stcOutputData_Delete(newDataObject);

      Serial.print("Get: ");
      Serial.println(inputString.substring(0, 35));
    }

    // Check counter to send on LoRa every 5 mins
    if ((now - lastSenderCounter > 300000) && (objectCounter > 15))
    {
      lastSenderCounter = now;

      // Get air direction value and set to average object
      averageDataObject->airDirection = getMostAppearValue(windDirectionValueArray, windDirectionCounterArray, 8);

      stcOutputData_ToDataString(averageDataObject, sendingDataStringBuffer);

      LoRa.beginPacket();
      LoRa.print(sendingDataStringBuffer);
      LoRa.endPacket();

      // Reset all object buffer
      memset((void *)windDirectionCounterArray, 0, sizeof(windDirectionCounterArray)); // windDirectionCounterArray
      objectCounter = 0;                                                               // objectCounter
      stcOutputData_Create(&averageDataObject);                                        // averageDataObject

      Serial.println("Sent");
    }

    inputString = "";
    inputString.setCharAt(33, '\0');
    inputString.setCharAt(34, '\0');
    stringComplete = false;
  }
}
