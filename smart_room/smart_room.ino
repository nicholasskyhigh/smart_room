//Smart Room
//smart room controller, controls hue lights and has different functions
//Nicholas Elkins

#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <OneButton.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <colors.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


Adafruit_BME280 bme;
Adafruit_NeoPixel pixels(12, 17, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
