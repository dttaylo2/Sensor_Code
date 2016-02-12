#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2   

line display

#include "EmonLib.h"                   // Include Emon Library

EnergyMonitor emon1;                   // Create an instance

void setup()

{  

  Serial.begin(9600);

  lcd.init();                      // initialize the lcd

  lcd.backlight();               // Turn on LCD Backlight

  emon1.current(1, 111.1);             // Current: input pin, calibration.

}

void loop()

{

  double Irms = emon1.calcIrms(1480);  // Calculate Irms only

  double I = (Irms/5);               // Compensate for five loops

  Serial.println(I);          // Irms

  lcd.setCursor(0,0);         // Set LCD position

  lcd.print("Current: ");

  lcd.print(I);

  }

