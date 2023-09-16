#include <Adafruit_STMPE610.h>

#include <Adafruit_TCS34725.h>

#include <SPI.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Adafruit_TCS34725.h"
#include <math.h>
#include "SparkFunMPL3115A2.h"
#include "Adafruit_CCS811.h"
#include "Adafruit_VEML6070.h"
rgb_lcd lcd;
double Temp = 99;
double Humidity = 30.3;
double AirQuality = 96.2;
double Pressure = 137.3;
int DELAY = 3000;
int r = 50;
int g = 10;
int b = 255;
int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 7;//Variable for Soil moisture Power
//soil moisture

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
//rgb sensor (need adafruit library (Adafruit TCS3472))


#define LIGHT_SENSOR A0//Grove - Light Sensor is connected to A0 of Arduino
float Rsensor; //Resistance of sensor in K

MPL3115A2 myPressure;
//SDA = A4 (use inline 10k resistor if your board is 5V)
//SCL = A5 (use inline 10k resistor if your board is 5V)
//Create an instance of the object

/*
  A new sensor requires at 48-burn in. Once burned in a sensor requires
  20 minutes of run in before readings are considered good.

  Hardware Connections (Breakoutboard to Arduino):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to A4
  SCL to A5
*/
Adafruit_CCS811 ccs;


Adafruit_VEML6070 uv = Adafruit_VEML6070();


void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }



  myPressure.begin(); // Get sensor online

  //Configure the sensor
  
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  uv.begin(VEML6070_1_T);  // pass in the integration time constant

}

void loop() {
  int soil_moisture = readSoil();

  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  readRGB(r, g, b);

  float lightIntensity = readLightIntensity();

  float pressure = readPressure();

  float temperature = readTemperature();

  uint16_t co2 = readCO2();
  uint16_t airquality = readTVOC();

  float UV = readUV();
  display();

  delay(100);
}

int readSoil()
{

    digitalWrite(soilPower, HIGH);//turn D7 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soilPin);//Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);//turn D7 "Off"
    return val;//send current moisture value
}

void readRGB(uint16_t &r, uint16_t &g, uint16_t &b) {
  tcs.getRawData(&r, &g, &b, nullptr);
}
float readLightIntensity()
{
  int sensorValue = analogRead(LIGHT_SENSOR);
  Rsensor = (float)(1023 - sensorValue) * 10 / sensorValue;
  return Rsensor;
}

float readPressure() {
  return myPressure.readPressure();;
}

float readTemperature() {
  return myPressure.readTempF();
}

uint16_t readCO2() {
  if (ccs.available() && !ccs.readData()) {
    return ccs.geteCO2();
  }
  return 0; // Return 0 if data is not available or there's an error
}

// Function to read TVOC from the sensor and return it
uint16_t readTVOC() {
  if (ccs.available() && !ccs.readData()) {
    return ccs.getTVOC();
  }
  return 0; // Return 0 if data is not available or there's an error
}

float readUV() {
  return uv.readUV();
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