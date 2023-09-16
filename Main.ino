#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;
double Temp = 99;
double Humidity = 30.3;
double AirQuality = 96.2;
double Pressure = 137.3;
int DELAY = 3000;
int r = 50;
int g = 10;
int b = 255;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  
}

void loop() {
    display();
}
void display() {
  // clear screen for the next loop:
    lcd.clear();
    // Turn off the display:
    lcd.noDisplay();
    lcd.setRGB(r,g,b);
    lcd.print("Temp:  ");
    lcd.print(Temp);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humid: ");
    lcd.print(Humidity);
    lcd.print("%");
    lcd.display();
    // Turn on the display:
    delay(DELAY);
    lcd.clear();
    lcd.noDisplay();
    lcd.setRGB(r,g,b);
    lcd.print("AQI:   ");
    lcd.print(AirQuality);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Press: ");
    lcd.print(Pressure);
    lcd.print("kPa");
    // Turn on the display:
    lcd.display();
    delay(DELAY);
}