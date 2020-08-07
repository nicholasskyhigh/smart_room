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

const int upper = 2;
const int lower = 3;
int lightnumber = 0;// bulb number 1-5 and sets it to 0
int Brightness;//brightness number 0-255 set by encoder
bool lights = false; //boolean to turn the lights on or off
bool changebright;
int color; //selects which color to set the bulbs

enum State {mode1, mode2, mode3};
State _state = mode1;

Adafruit_BME280 bme;
Adafruit_NeoPixel pixels(12, 4, NEO_GRB + NEO_KHZ800);
IPAddress ip(192,168,1,3);
OneButton button1(upper, false);
OneButton button2(lower, false);

void setup() {
  

  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  pixels.begin();
  pixels.clear();
  Serial.print("LinkStatus: ");
  Serial.println(Ethernet.linkStatus());
  Serial.println("Ready");
  button1.attachClick(button1click);
  button2.attachClick(button2click);
  button1.attachDoubleClick(button1doubleclick);
  button2.attachDoubleClick(button2doubleclick);
  button1.attachLongPressStart(button1longpressstart);
  button2.attachLongPressStart(button2longpressstart);
  restrt();

  
 
} 

void loop() {
  button1.tick();
  button2.tick();
  
}

void button1click() {

  
}

void button1doubleclick() {
  changebright = !changebright;
  
}

void button2click() { 
  lightpush();
  Serial.println("lightsPushed");
  if (lightnumber  == 0) {
    Serial.println("no lights on");
  }
}

void button2doubleclick() { // changes the mode

  switch (_state) {
    case mode1:
     _mode1();
     _state = mode2;
    break;
    case mode2:
     _mode2();
     _state = mode3;
    break;
    case mode3:
     _mode3();
     _state = mode1;
   
  }
  
}

void _mode1() {
 
 
  Serial.println("mode1");
  
  
}

void _mode2() {
  Serial.println("mode2");
  
}

void _mode3() {
  Serial.println("mode3");
  
}

void button1longpressstart() {
  lightnumber = lightnumber +1;
  Brightness = 255;
  Serial.print ("lightnumber: ");
  Serial.println(lightnumber);
}

void button2longpressstart() { // turns lights on/off state 0 = off
  lights = !lights;
  Serial.print("on:state ");
  Serial.println(lights);
}

void brightchange() {
  if (changebright == true) {
    pixels.clear();
    
    
  }
}













void lightpush() { //button2 code that pushes light info to the hue lights
  if (lightnumber == 1 ){
    setHue(1, lights, color, Brightness);
  }

  if (lightnumber == 2) {
    setHue(1, lights, color, Brightness);
    setHue(2, lights, color, Brightness);
  }

  if (lightnumber == 3) {
    setHue(1, lights, color, Brightness);
    setHue(2, lights, color, Brightness);
    setHue(3, lights, color, Brightness);
  }

  if (lightnumber == 4) {
    setHue(1, lights, color, Brightness);
    setHue(2, lights, color, Brightness);
    setHue(3, lights, color, Brightness);
    setHue(4, lights, color, Brightness);
  }

  if (lightnumber == 5) {
    setHue(1, lights, color, Brightness);
    setHue(2, lights, color, Brightness);
    setHue(3, lights, color, Brightness);
    setHue(4, lights, color, Brightness);
    setHue(5, lights, color, Brightness);
  }
}

void restrt() {
 
    setHue(1, false, color, Brightness);
    setHue(2, false, color, Brightness);
    setHue(3, false, color, Brightness);
    setHue(4, false, color, Brightness);
    setHue(5, false, color, Brightness);
    pixels.clear();
    pixels.show();
}
