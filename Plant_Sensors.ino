#include <Wire.h>
#include "TSL2561.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme: //I2C
//Adafruit_BME280 bme(BME_CS); //hardware SPI
//Adafruit_BME280 bme(BME_CS,BME_MOSI,BME_MISO,BME_SCK); //software SPI

TSL_2561 tsl(TSL2561_ADDR_FLOAT);

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (tsl.begin()){
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TSL sensor.");
    while(1);
  }
  if (bme.begin()){
    //Serial.println("Found sensor");
  } else {
    Serial.println("No BME Sensor");
    while(1);
  }

  //tsl.setGain(TSL2561_GAIN_0X); //set no gain (for bright situations)
  tsl.setGain(TSL2561_GAIN_16X); //set 16x gain (for dim situations)

  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS); //Shortest integration time (bright light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS); //Medium integration time (medium light)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS); //Longest integration time (low light)

  delay(100);
 }

void loop(void) {
  // put your main code here, to run repeatedly:
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);

  //Serial.println(x,DEC);

  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >>16;
  full = lum & 0xFFFF;

  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  Serial.println(ir);
  Serial.println(full);
  Serial.println(full - ir);
  Serial.println(tsl.calculateLux(full, ir));

  printValues();

  delay(10000);
}

void printValues() {
    //Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    //Serial.println(" *C");

    //Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    //Serial.println(" hPa");

    //Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");

    //Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    //Serial.println(" %");

    Serial.println();
}
