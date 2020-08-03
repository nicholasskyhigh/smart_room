// obstacles
// tests the functionality of the obstacle avoidance 
//Nicholas Elkins
//7/31/2020

//int led = 13;
//int board = 0;
//int reading;
//
//
//void setup() {
//  Serial.begin(9600);
//  while (!Serial);
//  pinMode(led,OUTPUT);
//  pinMode(board, INPUT);
//  // put your setup code here, to run once:
//
//}
//
//void loop() {
//  reading = analogRead(board);  
//  
//  // put your main code here, to run repeatedly:
//
//}

int LED = 13; // Use the onboard Uno LED
int isObstaclePin = 2;  // This is our input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  Serial.begin(9600);  
}

void loop() {
  isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW)
  {
    Serial.println("OBSTACLE!!, OBSTACLE!!");
    digitalWrite(LED, HIGH);
  }
  else
  {
    Serial.println("clear");
    digitalWrite(LED, LOW);
  }
  delay(200);
}
