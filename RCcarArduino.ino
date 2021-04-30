//https://www.tinkercad.com/things/8ReFUU2MswP
//Potentiometer is replaced with a sound detector.
//https://www.sparkfun.com/products/12642 Connect envelope to A0.

#include <Servo.h>

Servo servo;
bool crossthreshold;
int count; 
int threshold = 50;     //Threshold for audio peak.
int audiolength = 50;   //Lenght of audio peak (ms).

void setup(){
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  servo.attach(7);
}

void loop(){
  digitalWrite(10, HIGH);   //Enables H-bridge
  count = 0;
  if (detectclap()){
    count++;
    for (int i = 0; i < 1000; i++){
      delay(1);
      if (detectclap()){
        count++;
        i = 0;
      }
    }
    Serial.print(" Count: ");
    Serial.print(count);
  }
  if (count < 2){   //Car doesn't move until 2 claps are detcted.
    still();        //Other audiopeaks were often confused for a clap.
  }
  else if (count == 2){
    forward();
    delay(5000);
  }
  else if (count == 3){
    circle();
    delay(10000);
  }
}

//Detects claps, returns boolean.
bool detectclap(){
  int audio = analogRead(A0);
  int data[audiolength];
  if (audio < threshold){
    crossthreshold = true;
  }
  else if (audio > threshold){
    for (int i = 0; i < audiolength; i++){
      data[i] = analogRead(A0);
      Serial.println(data[i]);
      delay(1);
    }
    if (analyzedata(data) && crossthreshold){
      return true;
    }
    else {
      crossthreshold = false;
    }
  }
  return false;
}

//Analyzes audiodata for function detectclap, returns boolean.
bool analyzedata(int audiodata[]){
  for (int i = 0; i < audiolength; i++){
    if (audiodata[i] * 5 < audiodata[1]){
      return true;
    }
  }
  return false;
}

//Functions below control the motor and the servo.
//They are called depending on how many claps were detected.
//Still is the default state in which the car doesn't move.
//Servo.write(25) is straight. 0 and 50 are max values for the servo.
//digitalWrite(9, LOW); digitalWrite(8, HIGH); car goes forward.
//digitalWrite(8, LOW); digitalWrite(9, HIGH); car goes backwards.

void still(){
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  servo.write(25);
}

void forward(){
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  servo.write(25);
}

void circle(){
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  servo.write(0);
}

