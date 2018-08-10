#include <SPI.h>
#include "Bitmaps.h"
#include "LCD_Functions.h"

const int led = 13;

void showTimeScreen(void);
void showResultScreen(void);

void setup()
{
  pinMode(led, OUTPUT);

  Serial.begin(9600);

  lcdBegin();      // This will setup our pins, and initialize the LCD
  updateDisplay(); // with displayMap untouched, SFE logo
  setContrast(65); // Good values range from 40-60
  delay(2000);

  clearDisplay(WHITE);
  updateDisplay();
}

void loop()
{
  digitalWrite(led, HIGH);
  lightOff();
  setBitmap(Update);
  updateDisplay();
  delay(2000);

  digitalWrite(led, LOW);
  lightOn();
  setBitmap(Sun);
  updateDisplay();
  delay(2000);

  setBitmap(Rain);
  updateDisplay();
  delay(2000);
}

void showTimeScreen(int hour, int min, int temp, int humi)
{

}

void showResultScreen(int rain)
{
  if (rain > 0)
  {
    setBitmap(Rain);
    updateDisplay();
  }
  else
  {
    setBitmap(Sun);
    updateDisplay();
  }
}

