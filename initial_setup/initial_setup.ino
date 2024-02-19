#include <Servo.h>
#include "constants.h"

Servo hipL;
Servo hipR;
Servo kneeL;
Servo kneeR;
Servo ankleL;
Servo ankleR;

void setup() {
  // put your setup code here, to run once:
  hipL.attach(6);
  hipR.attach(7);
  kneeL.attach(4);
  kneeR.attach(5);
  ankleL.attach(2);
  ankleR.attach(3);

  hipL.write(hipLOffset);
  kneeL.write(kneeLOffset);
  ankleL.write(ankleLOffset);
  
  hipR.write(hipROffset);
  kneeR.write(kneeROffset);
  ankleR.write(ankleROffset);


}

void loop() {
  // put your main code here, to run repeatedly:
}
