#include <SPI.h>
#include "Bitmaps.h"
#include "LCD_Functions.h"

void setup()
{
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
  setBitmap(Update);
  updateDisplay();
  delay(2000);

  setBitmap(Sun);
  updateDisplay();
  delay(2000);

  setBitmap(Rain);
  updateDisplay();
  delay(2000);
}
