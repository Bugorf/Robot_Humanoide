#include <Servo.h>
#include "constants.h"
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(9, 8); // CE, CSN
char s = 'a';

const byte address[6] = "00001";

Servo hipL;
Servo hipR;
Servo kneeL;
Servo kneeR;
Servo ankleL;
Servo ankleR;

void updateServoPos(int target1, int target2, int target3, char leg){
  if (leg == 'l'){
    hipL.write(hipLOffset - target1);
    kneeL.write(kneeLOffset - target2);
    ankleL.write(2*ankleLOffset - target3);
  }
  else if (leg == 'r'){
    hipR.write(hipROffset + target1);
    kneeR.write(kneeROffset + target2);
    ankleR.write(target3);
  }
}

void pos(float x, float z, char leg){
  float hipRad2 = atan(x/z);
  float hipDeg2 = hipRad2 * (180/PI);

  float z2 = z/cos(hipRad2);

  float hipRad1 = acos((sq(l1) + sq(z2) - sq(l2))/(2*l1*z2));
  float hipDeg1 = hipRad1 * (180/PI);
  
  float kneeRad = PI - acos((sq(l1) + sq(l2) - sq(z2))/(2*l1*l2));

  float ankleRad = PI/2 + hipRad2 - acos((sq(l2) + sq(z2) - sq(l1))/(2*l2*z2));
  
  float hipDeg = hipDeg1 + hipDeg2;
  float kneeDeg = kneeRad * (180/PI);
  float ankleDeg = ankleRad * (180/PI);

 

  updateServoPos(hipDeg, kneeDeg, ankleDeg, leg);  
}

void takeStep(float stepLength, int stepVelocity){
  for (float i = stepLength; i >= -stepLength; i-=0.5){
    if (radio.available()) {
      char text[4] = "";
      radio.read(&text, sizeof(text));
    
      if (strlen(text) == 4) {
        Serial.println("stop");
        hipL.write(125);
        kneeL.write(kneeLOffset);
        ankleL.write(ankleLOffset);
  
        hipR.write(70);
        kneeR.write(kneeROffset);
        ankleR.write(ankleROffset);
      
      }
    
    }
    
    pos(i, stepHeight, 'r');
    pos(-i, stepHeight - stepClearance, 'l');
    delay(stepVelocity);
       
    
  }

  for (float i = stepLength; i >= -stepLength; i-=0.5){

    if (radio.available()) {
      char text[4] = "";
      radio.read(&text, sizeof(text));
    
      if (strlen(text) == 4) {
        Serial.println("stop");
        hipL.write(125);
        kneeL.write(kneeLOffset);
        ankleL.write(ankleLOffset);
  
        hipR.write(70);
        kneeR.write(kneeROffset);
        ankleR.write(ankleROffset);
      
      }
    
    }
    
    pos(-i, stepHeight - stepClearance, 'r');
    pos(i, stepHeight, 'l');
    delay(stepVelocity);
  }
       
}

// initialiser
void initialize(){
  for (float i = 10.7; i >= stepHeight; i-=0.1){
    pos(0, i, 'l');
    pos(0, i, 'r');
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  hipL.attach(6);
  hipR.attach(7);
  kneeL.attach(4);
  kneeR.attach(5);
  ankleL.attach(2);
  ankleR.attach(3);

  hipL.write(125);
  kneeL.write(kneeLOffset);
  ankleL.write(ankleLOffset);
  
  hipR.write(70);
  kneeR.write(kneeROffset);
  ankleR.write(ankleROffset);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  delay(1000);
  
 
}

void loop() {

  if (radio.available()) {
    char text[4] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    
    if (strlen(text) == 4) {
      Serial.println("no");
      hipL.write(125);
      kneeL.write(kneeLOffset);
      ankleL.write(ankleLOffset);
  
      hipR.write(70);
      kneeR.write(kneeROffset);
      ankleR.write(ankleROffset);
      
    } else if (strlen(text) == 2) {
      Serial.println("ok");
      takeStep(2,70);
    }
    
  }
 
  
 
}
