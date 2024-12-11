#include <Servo.h>  // the servo library

#define SERVOS 5 // the number of servos
#define SRV_CLOSE 0 // angle for closed finger
#define SRV_OPEN 180 // angle for open finger
int servoPins[SERVOS] = {8,10,9,11,12}; // servos on spatial pin 3 through 7 (thumb, index, middle, ring, pinky)
int servoMin[SERVOS] = {700,700,700,700,700}; // minimum pwm value for each servo
int servoMax[SERVOS] = {1600,2300,2300,2300,2300}; // maximum pwm value for each servo
int servoInv[SERVOS] = {1,0,0,0,0}; // indicates if servo direction is inverted
/*
int servoPins[SERVOS] = {};//{2,3,4,5,6}; // servos on pins 2 through 6 (thumb, index, middle, ring, pinky)
int servoMin[SERVOS] = {1000,544,544,544,544}; // minimum pwm value for each servo
int servoMax[SERVOS] = {1600,2400,2400,2400,2400}; // maximum pwm value for each servo
int servoInv[SERVOS] = {1,0,0,0,0}; // indicates if servo direction is inverted
//*/
unsigned long tmsServo_Attached[SERVOS]; // time since last servo attach. Used to reduce power comsumption
Servo myservo[SERVOS];
int servoRemap[SERVOS] = {0,1,2,3,4};
int ButtonPin = 7; //pushbutton connected to digital pin 7 - this will close hand into fist as a demo
int LED[6] = {8, 9, 10, 11, 12, 13};//  LEDPin connected to pin 8
int finalReading[SERVOS];
int t = 750; // this sets delay for finger to close
int thresh[SERVOS] = {55, 32, 80, 45, 30};

//------------------------------------------------------------
void setup(){
  Serial.begin(9600);
  for(int i = 0; i < SERVOS; i++){
    pinMode(LED[i], OUTPUT);
  }
  pinMode(ButtonPin, INPUT);
}
//------------------------------------------------------------
void loop(){
  if (digitalRead(ButtonPin) == HIGH){ // This will activate the TENS if the PushButton is pressed on S1
    moveAllFingers();
  } 
  for(int i = 0; i < SERVOS; i++){
    finalReading[i] = analogRead(i);
  }
  sendReadings();
  if (Serial.available() > 0) {
    byte buf [2];
    byte b = Serial.readBytes(buf, 2);
    if (b > 1) {
      if (buf[0] == 'F') {
        //   Serial.print("moveFInger");
        if (buf[1] < 5 ) {
          moveFinger(buf[1], 1);
          //     Serial.println(f);
          // digitalWrite(LED[f], HIGH); //sets all LEDs on
        } else {
          for (int i = 0; i < SERVOS; i++) {
            moveFinger (i, 0); // move all fingers
          }
        }
      } else if (buf[0] == 'f') {
        //  int f = Serial.read();
        if (buf[1] < 5 ) {
          moveFinger(buf[1], 0);
        }
      }
    }
  }/*
   if(Serial.available() > 0) {
    int inByte = Serial.read();
    if(inByte == 'F'){
      int f = Serial.read();
       // Serial.print("moveFInger");
      if (f < 5 ){
        moveFinger(f, 1);
     //   Serial.println(f);
        digitalWrite(LED[f], HIGH); //sets all LEDs on
      }else{
        openAllFingers();
      }
    }else if(inByte == 'f'){
      int f = Serial.read();
      if (f < 5 ){
        moveFinger(f, 0);
      }
    }
   }//*/
}
//------------------------------------------------------------
void sendReadings(){
  for(int i = 0; i < SERVOS-1; i++){
    Serial.print(finalReading[i]);
    Serial.print(",");
  }
  Serial.println(finalReading[SERVOS-1]);
}
//------------------------------------------------------------
void stimulate(int finger){
   // Serial.print("Stimulate");
    //Serial.print(finger);
    //Serial.println("!");
    digitalWrite(LED[finger], HIGH); //sets the LED on
    moveFinger (finger, 1); // Start HacherHAND v5.8
    delay(t);                  // waits for a second
    digitalWrite(LED[finger], LOW);  //sets the stim off
    moveFinger (finger, 0); // Start HacherHAND v5.8
}
//------------------------------------------------------------
void moveFinger(int finger, int pos){
  if(finger < SERVOS){
  finger = servoRemap[finger];
  int servoPos = abs(((pos==1)?SRV_CLOSE:SRV_OPEN) - SRV_OPEN*servoInv[finger]);
  if (myservo[finger].read() != servoPos) {
    if (!myservo[finger].attached()) {
      myservo[finger].attach(servoPins[finger], servoMin[finger], servoMax[finger]);
    }
    myservo[finger].write(servoPos);
  }
  tmsServo_Attached[finger] = millis();
  }
}
//------------------------------------------------------------
void openAllFingers(){
    for(int i = 0; i < SERVOS; i++){
      moveFinger (i, 0); // move all fingers
    }
    delay(t); //this delay is for leaving the LEDs turned on just a bit longer to make it easier to see.
    for(int i = 0; i < SERVOS; i++){
      digitalWrite(LED[i], LOW);  //sets the stim off
    }
  }
//------------------------------------------------------------
void moveAllFingers(){
    for(int i = 0; i < SERVOS; i++){
      moveFinger (i, 1); // close all fingers
      digitalWrite(LED[i], HIGH); //sets all LEDs on
    }
    delay(t);             // waits for delay
    for(int i = 0; i < SERVOS; i++){
      moveFinger (i, 0); // move all fingers
    }
    delay(t); //this delay is for leaving the LEDs turned on just a bit longer to make it easier to see.
    for(int i = 0; i < SERVOS; i++){
      digitalWrite(LED[i], LOW);  //sets the stim off
    }
  }

