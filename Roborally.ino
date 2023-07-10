#include "Motors.h"

uint16_t motorpins[6][4] = {{ 2,  5,  4,  3},
                            { 6,  9,  8,  7},
                            {46, 49, 48, 47},
                            {22, 25, 24, 23},
                            {36, 39, 38, 37},
                            {40, 43, 42, 41}};
Motors motors(&motorpins[0][0]);
//Motors motors;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Hello world!");

  motors.init();

  delay(2000);
  motors.move(1, 0);
  delay(2000);
  motors.move(-1, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  //Serial.println("Hello again!");
  //motors.move(1,0);
  delay(2000);
  //motors.move(-1,0);
}
