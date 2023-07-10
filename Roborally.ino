#include "Motors.h"


Motors motors;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Hello world!");

  motors.init();

  delay(1000);
  motors.move(1, 0);
  delay(1000);
  motors.move(-1, 0);
  delay(1000);
  motors.move(0, 1);
  delay(1000);
  motors.move(0, -1);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  //Serial.println("Hello again!");
  //motors.move(1,0);
  delay(2000);
  //motors.move(-1,0);
}
