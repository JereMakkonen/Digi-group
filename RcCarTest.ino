// https://www.tinkercad.com/things/8ReFUU2MswP 

#include <Servo.h>

Servo servo;
int sensorValue = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  servo.attach(7);
}

void loop()
{
  sensorValue = analogRead(A0);
  digitalWrite(10, HIGH);
  if (sensorValue > 200){
    circuit();
  }
  if (sensorValue == 0){
    stop();
  } 
}

void forward(){
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
}

void circuit(){
  forward();
  servo.write(30);
}

void stop(){
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  servo.write(0);
}