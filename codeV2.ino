//https://www.tinkercad.com/things/8ReFUU2MswP
//Potentiometer is replaced with a sound detector.
//https://www.sparkfun.com/products/12642 Connect envelope to A0.

#include <Servo.h>
#include <EEPROM.h>

Servo servo;
bool crossthreshold;
int count; 
int threshold = 40;     //Threshold for audio peak.
int audiolength = 50;   //Lenght of audio peak (ms).

void setup(){
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(11, OUTPUT);
  servo.attach(5);
  servo.write(85);
  analogWrite(11, EEPROM.read(0));   //Enables H-bridge with the pwm value stored in EEPROM. 
}


//Functions below control the motor and the servo.
//They are called depending on how many claps were detected.


void indicate(){
  servo.write(55);
  delay(2000);
  servo.write(105);
  delay(2000);
  servo.write(55);
  delay(2000);
  servo.write(105);
  still();
}


void still(){
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  servo.write(85);  
}

void forward(){
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay(2500);
  still();  
}

void backward(){
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(2500);
  still();
}

void left(){
  servo.write(55);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH); 
  delay(2500);
  still();  
}

void right(){
  servo.write(105);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay(2500);
  still();
}

void menu(){
//On 6 claps this menu is entered. When the manu entered the car's front wheels will move from side to side twice.
//After this the user can clap 1 to 3 times and change the default speed of the vehicle. 1 = normal, 2 = quick, 3 = full power.
   
  indicate();
  count = 0;
  while (count == 0){
    countclaps();
  }
  if (count == 1){
    EEPROM.update(0,175);
  }
  else if (count == 2){
    EEPROM.update(0,210);
  }
  else if (count == 3){
    EEPROM.update(0,255);
  }
  count = 0;
  indicate();
}

void states(){
  // It's called in the main loop to check on the amount of claps and act accordingly.
  if (count == 2){
    forward();
  }
  else if (count == 3){
    backward();
  }
  else if (count == 4){
    right();
  }
  else if (count == 5){
    left();
  }
  else if (count == 6){
    menu();
  }
}  

void loop(){
  //MAIN LOOP
  //Continuously waits for claps with the countclaps function and checks if said function has detected any claps.
  //If claps are detected an action will be performed
  if (count > 0){
     Serial.print(count);
  }
  countclaps(); 
  states();
}


bool detectclap(){
  //Detects claps, returns boolean.
  //For serial plotter.
  if (analogRead(A0) > threshold){
    return true;
  }
  return false;
}


int countclaps(){
  // counts the claps.
  count = 0;
  //the function starts waiting for a clap.
  if (detectclap() == true){
    count++; 
    delay(500); 
    for (int i = 0; i < 1000; i++){
      delay(1);
      if (detectclap() == true){
        count++;
        i = 0;
        delay(500); 
      }
    }
  }
}
