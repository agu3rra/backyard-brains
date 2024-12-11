#define EMG A0

int reading[10];
int finalReading;
byte multiplier = 1;

void setup(){
  Serial.begin(9600);
}

void loop(){
  for(int i = 0; i < 10; i++){
    reading[i] = analogRead(EMG) * multiplier;
    delay(2);
  }
  for(int i = 0; i < 10; i++){
    finalReading += reading[i];
  }
  finalReading /= 10;
  Serial.print("Value = ");
  Serial.println(finalReading);
  delay(100);
}
