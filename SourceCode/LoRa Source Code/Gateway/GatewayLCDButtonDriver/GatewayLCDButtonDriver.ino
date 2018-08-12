#include <SPI.h>
#include "TimerOne.h"
#include "Bitmaps.h"
#include "string.h"
#include "LCD_Functions.h"

#define INPUT_STRING_SIZE 255

const int led = 13;
int ledStatus = LOW;
bool stringComplete = false;

struct
{
  char str[INPUT_STRING_SIZE];
  int currentIndex;
} inputString;

typedef struct
{
  int rainfall;
} resultScreenInput_t;

typedef struct
{
  int hour;
  int min;
  int temp;
  int humi;
} timeScreenInput_t;

void showLoadingScreen(void);
void showTimeScreen(int hour, int min, int temp, int humi);
void showResultScreen(int rain);
void timerOneInterruptHandler(void);
int checkInputString(void);
void clearInputString(void);
void getTimeScreenInputFromInputString(timeScreenInput_t *inputs);
void getResultScreenInputFromInputString(resultScreenInput_t *inputs);

void setup()
{
  clearInputString();

  pinMode(led, OUTPUT);

  Serial.begin(115200);

  Timer1.initialize(1000000); // 1s
  Timer1.attachInterrupt(timerOneInterruptHandler);

  lcdBegin(); // This will setup our pins, and initialize the LCD
  lightOn();
  updateDisplay(); // with displayMap untouched, SFE logo
  setContrast(65); // Good values range from 40-60
  delay(1000);

  showLoadingScreen();
  updateDisplay();
}

void loop()
{
  if (stringComplete)
  {
    switch (checkInputString())
    {
    case 1:
    {
      resultScreenInput_t inputs;
      getResultScreenInputFromInputString(&inputs);
      showResultScreen(inputs.rainfall);
    }
    break;
    case 2:
    {
      timeScreenInput_t inputs;
      getTimeScreenInputFromInputString(&inputs);
      showTimeScreen(inputs.hour, inputs.min, inputs.temp, inputs.humi);
    }
    break;
    default:
      showLoadingScreen();
      break;
    }

    clearInputString();
  }
}

void showLoadingScreen(void)
{
  setBitmap(Update);
  updateDisplay();
}

void showTimeScreen(int hour, int min, int temp, int humi)
{
  char buffer[25];

  clearDisplay(0);

  // Time
  snprintf(buffer, 25, "Time:   %.2d:%.2d", hour, min);
  setStr(buffer, 0, 12, 1);

  // Temperature
  snprintf(buffer, 25, "Temp:  %.2d deg", temp);
  setStr(buffer, 0, 20, 1);

  // Humidity
  snprintf(buffer, 25, "Humi:     %.2d%%", humi);
  setStr(buffer, 0, 28, 1);

  updateDisplay();
}

void showResultScreen(int rain)
{
  char buffer[25];
  if (rain > 0)
  {
    setBitmap(Rain);

    snprintf(buffer, 25, "Rain: %.4dmm", rain);
    setStr(buffer, 5, 40, 1);
  }
  else
  {
    setBitmap(Sun);
    setStr("No rain", 21, 40, 1);
  }

  updateDisplay();
}

void timerOneInterruptHandler(void)
{
  digitalWrite(led, ledStatus);
  ledStatus = (ledStatus == LOW) ? HIGH : LOW;
}

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString.str[inputString.currentIndex++] = inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

int checkInputString(void)
{
  if ((inputString.currentIndex == 10) && (inputString.str[0] == 'C') && (inputString.str[1] == '0') && (inputString.str[2] == '1') && (inputString.str[3] == 'R'))
  {
    return 1;
  }
  else if ((inputString.currentIndex == 17) && (inputString.str[0] == 'C') && (inputString.str[1] == '0') && (inputString.str[2] == '2') && (inputString.str[3] == 'H') && (inputString.str[6] == 'M') && (inputString.str[9] == 'T') && (inputString.str[12] == 'H'))
  {
    return 2;
  }

  return 0;
}

void clearInputString(void)
{
  memset(inputString.str, 0, INPUT_STRING_SIZE);
  inputString.currentIndex = 0;
  stringComplete = false;
}

void getTimeScreenInputFromInputString(timeScreenInput_t *inputs)
{
  char hourStr[3];
  char minStr[3];
  char tempStr[3];
  char humiStr[3];

  memset((void*)hourStr, 0, sizeof(hourStr));
  memset((void*)minStr, 0, sizeof(minStr));
  memset((void*)tempStr, 0, sizeof(tempStr));
  memset((void*)humiStr, 0, sizeof(humiStr));

  if (inputs != NULL)
  {
    strncpy(hourStr, inputString.str + 4, sizeof(hourStr) - 1);
    strncpy(minStr, inputString.str + 7, sizeof(minStr) - 1);
    strncpy(tempStr, inputString.str + 10, sizeof(tempStr) - 1);
    strncpy(humiStr, inputString.str + 13, sizeof(humiStr) - 1);

    inputs->hour = (int) atoi(hourStr);
    inputs->min = (int) atoi(minStr);
    inputs->temp = (int) atoi(tempStr);
    inputs->humi = (int) atoi(humiStr);
  }
}

void getResultScreenInputFromInputString(resultScreenInput_t *inputs)
{
  char rainfallStr[5];

  memset((void*)rainfallStr, 0, sizeof(rainfallStr));

  if (inputs != NULL)
  {
    strncpy(rainfallStr, inputString.str + 4, sizeof(rainfallStr) - 1);
    inputs->rainfall = (int) atoi(rainfallStr);
  }
}
