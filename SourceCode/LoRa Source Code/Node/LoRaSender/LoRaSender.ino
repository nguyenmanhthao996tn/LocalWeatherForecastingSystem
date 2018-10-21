/*********** LIBRARY ***********/
#include <SPI.h>
#include <LoRa.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "TimerOne.h"
#include "OutputData.h"

/*********** DATA TYPE ***********/
typedef struct
{
  char str[128];
  int currentIndex;
} inputString_t;

/*********** CONSTANT ***********/
#define NodeID 123
const int led = 9;
const uint16_t windDirectionValueArray[] = {0, 45, 90, 135, 180, 225, 270, 315};
const int hardResetPin = A5;
const int chargeEnablePin = A2;
const int batteryPercentagePin = A3;


/*********** GLOBAL VARIABLE ***********/
inputString_t inputString;
char sendingDataStringBuffer[255];
volatile boolean stringComplete = false, getObjectFlag = false, sendObjectFlag = false;
volatile unsigned long timerOneCounter;

int ledState = LOW;

uint16_t windDirectionCounterArray[] = {0, 0, 0, 0, 0, 0, 0, 0};
stcOutputData_t *newDataObject;
stcOutputData_t *sendingDataObject;

/*********** METHOD HEADING ***********/
void clearInputString(void);
float addToAverage(float averageValue, uint16_t counter, uint16_t newValue);
uint16_t getMostAppearValue(const uint16_t valueArray[], uint16_t counterArray[], uint16_t size);
void saveNewObjectToSendingObject(stcOutputData_t *newDataObject);
void clearSendingObject(void);
void disableUnusedPeripheral(void);
void sleepNow(void);
void wakeNow(void);
void timerOneInterruptHandler(void);
void getMessageFromStc(void);
void sendMessageToGateway(void);
uint8_t getBatteryPercentage(void);
void setChargingState(uint8_t chargeEnable);
void hardResetFunction(void);
void wdtConfig(void);

/*********** MAIN ***********/
void setup()
{
  wdt_disable();

  pinMode(hardResetPin, INPUT);

  pinMode(chargeEnablePin, OUTPUT);
  digitalWrite(chargeEnablePin, HIGH); // enable

  pinMode(led, OUTPUT);
  digitalWrite(led, ledState);

  pinMode(4, INPUT);
  pinMode(5, INPUT);

  stcOutputData_Create(&sendingDataObject);
  clearSendingObject();
  clearInputString();

  Serial.begin(9600);
  while (!Serial)
    ;

  timerOneCounter = 0;
  Timer1.initialize(1000000); // 1s
  Timer1.attachInterrupt(timerOneInterruptHandler);

  LoRa.setPins(10, 8, 2);
  if (!LoRa.begin(868300000))
  {
    while (1)
      ;
  }

  LoRa.setSignalBandwidth(125000);
  LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(12);
  LoRa.setPreambleLength(8);
  LoRa.setSyncWord(0x52);
  LoRa.enableCrc();

  delay(500);

  LoRa.beginPacket();
  LoRa.print(NodeID);
  LoRa.endPacket();

  Serial.println(NodeID);

  wdtConfig();

  sleepNow();
}

void loop()
{
  if (getObjectFlag)
  {
    if ((inputString.str[33] == '\r') && (inputString.str[34] == '\n'))
    {
      wdt_reset();

      getObjectFlag = false;

      getMessageFromStc();
    }
  }

  if (sendObjectFlag)
  {
    sendObjectFlag = false;

    sendMessageToGateway();

    hardResetFunction();
  }

  sleepNow();
}

/*********** METHOD DEFINE ***********/
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString.str[inputString.currentIndex++] = inChar;
    digitalWrite(led, ledState);
    ledState ^= HIGH;
    if (inChar == '\n')
    {
      if (getObjectFlag)
      {
        wakeNow();
      }
      else
      {
        clearInputString();
      }
    }
  }
}

void timerOneInterruptHandler(void)
{
  timerOneCounter++;

  if ((timerOneCounter % 5) == 0) // 5 sec
  {
    getObjectFlag = true;
  }

  if (timerOneCounter > 300) // 5 min
  {
    timerOneCounter = 0;

    sendObjectFlag = true;
    wakeNow();
  }
}

void clearInputString(void)
{
  memset(inputString.str, 0, 128);
  inputString.currentIndex = 0;
}

float addToAverage(float averageValue, uint16_t counter, uint16_t newValue)
{
  float tempValue = (averageValue * counter) + newValue;
  return (tempValue / (counter + 1));
}

uint16_t getMostAppearValue(const uint16_t valueArray[], uint16_t counterArray[], uint16_t size)
{
  uint16_t maxCounterIndex = 0;
  uint16_t i;
  for (i = 1; i < size; i++)
  {
    if (valueArray[maxCounterIndex] < counterArray[i])
    {
      maxCounterIndex = i;
    }
  }

  return valueArray[maxCounterIndex];
}

void saveNewObjectToSendingObject(stcOutputData_t *newDataObject)
{
  // airSpeed1Min
  sendingDataObject->airSpeed1Min = newDataObject->airSpeed1Min;

  // airSpeed5Min
  sendingDataObject->airSpeed5Min = newDataObject->airSpeed5Min;

  // temperature
  sendingDataObject->temperature = newDataObject->temperature;

  // rainfall1Hour
  sendingDataObject->rainfall1Hour = newDataObject->rainfall1Hour;

  // rainfall24Hour
  sendingDataObject->rainfall24Hour = newDataObject->rainfall24Hour;

  // humidity
  sendingDataObject->humidity = newDataObject->humidity;

  // atmosphere
  sendingDataObject->atmosphere = newDataObject->atmosphere;
}

void clearSendingObject(void)
{
  sendingDataObject->airSpeed1Min = 0;
  sendingDataObject->airSpeed5Min = 0;
  sendingDataObject->temperature = 0;
  sendingDataObject->rainfall1Hour = 0;
  sendingDataObject->rainfall24Hour = 0;
  sendingDataObject->humidity = 0;
  sendingDataObject->atmosphere = 0;
}

void disableUnusedPeripheral(void)
{
  // power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  // power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
}

void sleepNow(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  disableUnusedPeripheral();
  sleep_mode();
}

void wakeNow(void)
{
  sleep_disable();
  power_all_enable();
}

void getMessageFromStc(void)
{
  inputString.str[35] = '\0';

  // Create and parse new object
  stcOutputData_Parse(inputString.str, &newDataObject);

  // Add values from new object to average object
  saveNewObjectToSendingObject(newDataObject);
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
  // stcOutputData_Delete(newDataObject); // No need to delete because there is no malloc new memory if the point is not equal to NULL

  Serial.print("Get: ");
  Serial.println(inputString.str);

  clearInputString();
}

void sendMessageToGateway(void)
{
  // Get air direction value and set to average object
  sendingDataObject->airDirection = getMostAppearValue(windDirectionValueArray, windDirectionCounterArray, 8);

  // Send to Gateway
  stcOutputData_ToDataString(sendingDataObject, sendingDataStringBuffer, NodeID, 0, 0, getBatteryPercentage());

  LoRa.beginPacket();
  LoRa.print(sendingDataStringBuffer);
  LoRa.endPacket();
  LoRa.sleep();

  // Reset all object buffer
  memset((void *)windDirectionCounterArray, 0, sizeof(windDirectionCounterArray));
  clearSendingObject();

  Serial.print("Sent: ");
  Serial.println(sendingDataStringBuffer);
}

uint8_t getBatteryPercentage(void)
{
  uint8_t returnValue = 0;
  uint16_t adcValue = analogRead(batteryPercentagePin);
  float batteryVoltage = (6.6 / 1023.0) * adcValue;

  return (uint8_t)((batteryVoltage - 3.2) * 20);
}

void setChargingState(uint8_t chargeEnable)
{
  if (chargeEnable) {
    digitalWrite(chargeEnablePin, HIGH);
  } else {
    digitalWrite(chargeEnablePin, LOW);
  }
}

void hardResetFunction(void)
{
  pinMode(hardResetPin, OUTPUT);
  digitalWrite(hardResetPin, LOW);
}

void wdtConfig(void)
{
  cli();       //  __disable_interrupt();
  wdt_reset(); //  __watchdog_reset();
  /* Start timed sequence */
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  /* Set new prescaler(time-out) value = 1024K cycles (~8 s) */
  WDTCSR = (1 << WDE) | (1 << WDIE) | (1 << WDP3) | (1 << WDP0);
  wdt_reset(); //  __watchdog_reset();
  sei();       //  __enable_interrupt();
}

ISR(WDT_vect)
{
  Serial.println("WDT_vect");
}
