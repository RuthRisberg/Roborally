#include "Motors.h"


Motors motors;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Hello world!");

  motors.init();

  delay(10000);
  Serial.println("Start homing");

  motors.home();

  /*
  delay(1000);
  motors.move(1, -1);
  delay(1000);
  motors.move(-1, 1);
  delay(1000);
  motors.move(1, 1);
  delay(1000);
  motors.move(-1, -1);
  */
  Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}
