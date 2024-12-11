#define NUM_LED 6  //sets the maximum numbers of LEDs
#define MAX_Low 10 //for people with low EMG activity
#define MAX_High 30//for people with high EMG activity
#define Threshold 3 // this sets the light to activate TENS
#define EMG A0  //we'l use A0 as the analog input signal for the EMG activity

int reading[10];
int finalReading;
int StimPin = 3; // TENS Digital 3 - (Transcutaneous Electrical Nerve Stimulator) - Relay output in Muscle Spiker Shield
int SwitchPin = 4;   // pushbutton connected to digital pin 4 -  this will momentarily turn on TENS
int SwitchThreshold = 7; //pushbutton connected to digital pin 7 - this will switch thresholds
int SwitchState = 0;
int SwitchThresholdState = 0;
int MAX = 0;
byte litLeds = 0;
byte multiplier = 1;
byte leds[] = {8, 9, 10, 11, 12, 13};


void setup(){
  Serial.begin(9600); //begin serial communications
  pinMode(StimPin, OUTPUT); // Set TENS output to StimPin - this one is the relay switch
  pinMode(SwitchPin, INPUT);      // sets the digital pin 4 as the switch input
  pinMode(SwitchThreshold, INPUT);      // sets the digital pin 4 as the switch input
  for(int i = 0; i < NUM_LED; i++){ //initialize LEDs as outputs
    pinMode(leds[i], OUTPUT);
  }
  MAX = MAX_High; //This sets the default to people with high EMG activity.
}

void loop(){
    //note the serial print stuff is sjust debugging tools
    SwitchThresholdState = digitalRead(SwitchThreshold);
    if (SwitchThresholdState == HIGH){ // This will allow the switching between a low threshold and high threshold state on S2
      //Serial.println("button hit");
      if (MAX == MAX_High){
        MAX = MAX_Low;
      }
      else{
        MAX = MAX_High;
      } 
      while (SwitchThresholdState == HIGH) { // This will pause the program while the person is touching the threshold button, 
        SwitchThresholdState = digitalRead(SwitchThreshold);  
        delay(10); 
      }                         // so it doesn't flip back and forth while button is pushed (aka debounce)
    }

    SwitchState = digitalRead(SwitchPin);
    //Serial.println(SwitchState);
    if (SwitchState == HIGH){ // This will activate the TENS if the PushButton is pressed on S1
      digitalWrite(StimPin, HIGH); 
      //Serial.println('Stim');
      while (SwitchState == HIGH) { // This will pause the program while the person is touching TENS test button
        SwitchState = digitalRead(SwitchPin);  
        delay(10);
      }    
    } 
  
    //READ EMG data
    for(int i = 0; i < 10; i++){    //take ten readings in 2 miliseconds
      reading[i] = analogRead(EMG) * multiplier;
      delay(2);
    }
    for(int i = 0; i < 10; i++){   //average the ten readings
      finalReading += reading[i];
    }
    finalReading /= 10;
    for(int j = 0; j < NUM_LED; j++){  //write all LEDs low and stim pin low; 
      digitalWrite(leds[j], LOW);
      digitalWrite(StimPin, LOW); 
    }
    Serial.print("EMG value: ");
    Serial.println(finalReading);
    Serial.print("\t");
    finalReading = constrain(finalReading, 0, MAX);
    litLeds = map(finalReading, 0, MAX, 0, NUM_LED);
    //Serial.println(litLeds);
    
    for(int k = 0; k < litLeds; k++){
      digitalWrite(leds[k], HIGH); // This turns on the LEDS
      if (k >= Threshold){ // defined at the top; may vary from 1 to 6
        digitalWrite(StimPin, HIGH); // This turns on the TENS if Threshold has been reached.
      } 
    }
    //delay(10);
    //for serial debugging, uncomment the next two lines.
    //Serial.println(finalReading);
    //delay(100);
}
    
