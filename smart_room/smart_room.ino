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

const int upper = 2;  //button1 
const int lower = 3;  //button2
const int A = 23;     //Wire for encoder
const int B = 22;     //Wire 2 for encoder
int lightnumber = 0;  // bulb number 1-5 and sets it to 0
int Brightness;       //brightness number 0-255 set by encoder
bool lights = false;  //boolean to turn the lights on or off
bool changebright = false;    //boolean to go into brightness change
int color;            //selects which color to set the bulbs
int mycolors[] = {red,orange,yellow,green,blue,indigo,purple};
int colorselect;
int pos;              //positon of the encoder
bool Mode1 = false;
bool Mode2 = false;
bool Mode3 = false;

enum State {mode1, mode2, mode3};
State _state = mode1;

Adafruit_BME280 bme;
Adafruit_NeoPixel pixels(12, 4, NEO_GRB + NEO_KHZ800);
IPAddress ip(192,168,1,3);
OneButton button1(upper, false);
OneButton button2(lower, false);
Encoder myEnc(B,A);
void setup() {
  

  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  pixels.begin();
  pixels.clear();
  Serial.print("LinkStatus: ");
  Serial.println(Ethernet.linkStatus());
  Serial.println("Connected");
  button1.attachClick(button1click);
  button2.attachClick(button2click);
  button1.attachDoubleClick(button1doubleclick);
  button2.attachDoubleClick(button2doubleclick);
  button1.attachLongPressStart(button1longpressstart);
  button2.attachLongPressStart(button2longpressstart);
  restrt();
  Serial.println("Ready");

  
 
} 

void loop() {
  button1.tick();
  button2.tick();
  poscheck();
  checks();
  
}
 
void button1click() {

  color = colorselect;
  Serial.print(color);
}

void button1doubleclick() {
  changebright = !changebright;
  Serial.print("Brightness change: ");
  Serial.println(Brightness);
  pixels.clear();
  pixels.show();
  
}

void button2click() { 
  lightpush();
  Serial.println("lightsPushed: ");
  if (lightnumber  == 0) {
    Serial.print("no lights on");
  }
}

void button2doubleclick() { // changes the mode

  switch (_state) {
    case mode1:
    
     Serial.println("Mode1");
     
     Mode1 = !Mode1;
     Mode2 = false;
     Mode3 = false;
     Serial.println(Mode1);
     Serial.println(Mode3);
     _state = mode2;
    break;
    case mode2:
    Mode2 = !Mode2;
    Mode1 = false;
    Mode3 = false;
     pixels.clear();
     pixels.show();
     Serial.println("Mode2");
     Serial.println(Mode2);
     _state = mode3;
    break;
    case mode3:
    Mode1 = false;
    Mode2 = false;
    pixels.clear();
    pixels.show();
    Mode3 = !Mode3;
     Serial.println(Mode3);
     _state = mode1;
     break;
  }
}


void button1longpressstart() {
  lightnumber = lightnumber +1;
  
  Serial.print("lightnumber: ");
  Serial.println(lightnumber);
}

void button2longpressstart() { // turns lights on/off state 0 = off
  lights = !lights;
  Serial.print("on:state ");
  Serial.println(lights);
}

void brightchange() { //activated with button1 double click to change brightness
  pixels.clear();
  int i = 0;
  for( i=0; i<12; i++){
    pixels.setPixelColor(i, white);
  }
  Brightness = map(pos, 0,96,0,255);
  pixels.setBrightness(Brightness);
  pixels.show();
  }



void poscheck() {          //checks the position of the encoder
  pos = myEnc.read();
}


void checks() {
  if (changebright == true) {
    brightchange();
  }

    
  
  if (Mode1 == true) {
    Mode2 = false;
    Mode3 = false;
    pixels.clear();
    pixels.setBrightness(Brightness);
    pixels.setPixelColor(2, blue);
    pixels.show();
  }

  if (Mode2 == true) {
    Mode1 = false;
    Mode3 = false;
     pixels.clear();
    pixels.show();
  }




  
  if (Mode3 == true) {
    int i3;
    int s = map(pos, 0,96,0,5);
    for (i3=0; i3<12; i3++) {
      pixels.setPixelColor(i3,mycolors[s]);
    }
    colorselect = HueRainbow[s];
   
    Mode2 = false;
    Mode1 = false;
    
    
    pixels.show(); 

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
