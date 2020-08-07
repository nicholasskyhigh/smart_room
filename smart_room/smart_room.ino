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
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

const int upper = 2;  //button1 
const int lower = 3;  //button2
const int A = 23;     //Wire for encoder
const int B = 22;     //Wire 2 for encoder
const int MPU=0x68;   //gyroscope although it appears accellerometer readings are actually gryo readings
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; //the adresses for the gyro/accel. output
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
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);


void setup() {
  

  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  pixels.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(50);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
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
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  unsigned status;
  status = bme.begin(0x76, &Wire);
    if (!status) {
      Serial.print("Bme error");
    }
  Serial.println("Ready");

  
 
} 

void loop() {
  button1.tick();
  button2.tick();
  poscheck();
  checks();
  
}
 
void button1click() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print ("Color Selected");
  color = colorselect;
  Serial.print(color);
  display.display();
  delay(750);
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
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.println ("LIGHTS PUSHED!");
  display.display();
  Serial.println("lightsPushed: ");
  if (lightnumber  == 0) {
  Serial.print("no lights on");cf
  }
  delay(1000);
}

void button2doubleclick() { // changes the mode
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.println (_state);
  display.display();
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
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.println("Number of lights on");
  display.print (lightnumber);
  display.display();
  lightnumber = lightnumber +1;
  
  Serial.print("lightnumber: ");
  Serial.println(lightnumber);  
  delay(1000);
}

void button2longpressstart() { // turns lights on/off state 0 = off
  lights = !lights;
 display.clearDisplay();
  display.display();
  display.setCursor(0,0);
  display.println("Lights on/off");
  display.display();
  delay(1000);
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
    if (changebright == true) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print ("Brightness changed to ");
    display.println(Brightness);
    display.display();
  }
  else {
    display.clearDisplay();
    display.display();
  }
  }



void poscheck() {          //checks the position of the encoder
  pos = myEnc.read();
}


void checks() { //used to check the state & clarify it, also ran in void loop, allowing it to function properly
  if (changebright == true) {
    brightchange();
  }

    
  
  if (Mode1 == true) {
    if(changebright == false){
       display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.print ("Mode: ");
    display.println (_state);
    display.print ("X Value = ");
    display.println (AcX);
    display.display();
    }
      
    Mode2 = false;
    Mode3 = false;
    int i1;
    int rotate;
    
   
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,12,true);  
    AcX=Wire.read()<<8|Wire.read();    

    if (AcX > -15000) {
      rotate = 0;
    }
    if (AcX > -9000) {
      rotate = 1 ;
    }
    if (AcX > -4500) {
      rotate = 2;
    }
    if (AcX > 0 ) {
      rotate = 3;
    }
    if (AcX > 4000) {
      rotate = 4;
    }
    if (AcX > 9000) {
      rotate = 5;
    }
    if (AcX > 14000) {
      rotate = 6;
    }
    
     for (i1 = 0; i1<12 ; i1++) {
      pixels.setPixelColor(i1,mycolors[rotate]);
     }
     pixels.show();
     colorselect = HueRainbow[rotate];
    }

  if (Mode2 == true) {
    float tempc;      //bme reads in celsius
    float tempf;
    int circle;     //variable for map
    int i2;         //variable for mode2
    Mode1 = false;
    Mode3 = false;

   

    
    tempc = bme.readTemperature();
    
    tempf = (tempc *9 / 5 + 32);
    circle = map(tempf, 80, 90,0,6);
    for (i2 = 0; i2 <12 ; i2++) {
    pixels.setPixelColor(i2,mycolors[circle]);
    }
//    Serial.println(circle);
  
    colorselect = HueRainbow[circle];
        pixels.show(); 
    if(changebright == false){
      display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.print ("Mode: ");
    display.println (_state);
    display.print ("Temp = ");
    display.println (tempf);
    display.display();
    }
    
  }




  
  if (Mode3 == true) {
    int i3;  //variable for mode3
    int s = map(pos, 0,96,0,5);

    if(changebright == false){
       display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.print ("Mode: ");
    display.println (_state);
    display.print ("Color = ");
    display.println (s);
    display.display();
    }
   
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
    display.clearDisplay();
    display.display();
}
