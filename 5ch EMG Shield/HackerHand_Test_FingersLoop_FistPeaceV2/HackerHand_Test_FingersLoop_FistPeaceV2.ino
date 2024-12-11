#include <Servo.h>  // the servo library

#define SERVOS 5 // the number of servos
#define SRV_CLOSE 0 // angle for closed finger
#define SRV_OPEN 180 // angle for open finger

int servoPins[SERVOS] = {8,10,9,11,12}; // servos on non obvious pins(thumb, index, middle, ring, pinky)
int servoMin[SERVOS] = {544,544,544,544,544}; // minimum pwm value for each servo
int servoMax[SERVOS] = {1600,2400,2400,2400,2400}; // maximum pwm value for each servo
int servoInv[SERVOS] = {1,0,0,0,0}; // indicates if servo direction is inverted
unsigned long tmsServo_Attached[SERVOS]; // time since last servo attach. Used to reduce power comsumption
unsigned long previousTime = 0;
Servo myservo[SERVOS];

int ButtonPress = 2; //pushbutton connected to digital pin 7 - this will close hand into fist as a demo
int ButtonState = 0; //declares button
int PeacePin = 13;   // pushbutton connected to digital pin 4 -  switch between peace and anger
int PeaceState = 0; //declaring state of Peace pin


void setup()
{
  //Serial.begin(9600);

  // Open Hand
  for (int f=0; f < SERVOS; f++)
    moveFinger (f, 0);
  
  // Wait for servos to respond
  delay (500);
  
  // Relax servos
  for (int f=0; f < SERVOS; f++)
    myservo[f].detach();
    
  delay (2000);
  pinMode(ButtonPress, INPUT);      // sets the digital pin 7 as the threshold changer
  pinMode(PeacePin, INPUT); 
}

void loop()
{
  unsigned long time=0, pos=0, f=0;
  
  ButtonState = digitalRead(ButtonPress); // this checks if button is pressed to allow hand to become fist
  if (ButtonState == HIGH)  // This will activate the TENS if the PushButton is pressed on S1
  {
    for (int i=0; i < SERVOS; i++)
      moveFinger (i, 1); // close all fingers

    delay(1000);    // waits for delay
    for (int i=0; i < SERVOS; i++)
      moveFinger (i, 0); // open all fingers
    
    delay(4000);   // waits for delay
  }
  
  for (int i=0; i < SERVOS; i++) {
    // Detach servo if time since last attach exceeds 750 ms.
    if (millis() - tmsServo_Attached[i] > 750) {
      myservo[i].detach();
    }
  }
  
     PeaceState  = digitalRead(PeacePin); // this checks if button is pressed to allow hand to become fist
     if (PeaceState == HIGH){ // This will activate the TENS if the PushButton is pressed on S1
        
        moveFinger (0, 1); // close all fingers 
        moveFinger (1, 1);
        moveFinger (2, 1);
        moveFinger (3, 1);
        moveFinger (4, 1);
        delay(1000);  
        moveFinger (1, 0); // peace
        moveFinger (2, 0); // peace
        delay(1000);                  // waits for delay
        moveFinger (0, 0); // move all fingers 
        moveFinger (3, 0);    
        moveFinger (4, 0);
        delay(500);
      }  
     
  
  time = millis();  
  if (time - previousTime > 250)
  {
    Serial.print("Entro - Finger/Pos: ");
    f=random(0,5);
    pos = random(0,2);
    Serial.print(f);
    Serial.print("/");
    Serial.println(pos);
    moveFinger (f, pos);
    previousTime = time;
  }
}

void moveFinger(int finger, int pos)
{
  if (pos == 1) { // Close
    if (myservo[finger].read() != abs(SRV_CLOSE - SRV_OPEN*servoInv[finger])) {
      // Close finger
      if (!myservo[finger].attached()) {
        myservo[finger].attach(servoPins[finger], servoMin[finger], servoMax[finger]);
        tmsServo_Attached[finger] = millis();
      }
      myservo[finger].write(abs(SRV_CLOSE - SRV_OPEN*servoInv[finger]));
    }
  }
  else { // Open
    if (myservo[finger].read() != abs(SRV_OPEN - SRV_OPEN*servoInv[finger])) {
      // Open finger
      if (!myservo[finger].attached()) {
        myservo[finger].attach(servoPins[finger], servoMin[finger], servoMax[finger]);
        tmsServo_Attached[finger] = millis();
      }
      myservo[finger].write(abs(SRV_OPEN - SRV_OPEN*servoInv[finger]));
    }
  }
}
