/*
rules (only selected elementss implemented):
https://www.thealexandrian.net/creations/roborally/roborally-ultimate-collection-rulebook.pdf

*/

#include "Game.h"

//Game game;
Controls controls;


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Hello world!");

  /* 
  // Init only the motors object
  motors.init();
  motors.home();
  */

  // Init the object that handles a motors object
  controls.init();
  controls.home();

  Serial.println("Setup done");
}

void loop() {
  /*
  // Run only the motors object
  motors.moveTo(0, 0, 1000, 0);
  motors.moveTo(0, 0, 0, 0);
  motors.moveTo(0, 0, 0, 800);
  motors.moveTo(0, 0, 0, 0);
  */

  // arguments are x,y,up,angle
  controls.moveTo(1,1,false,0); // one corner of the field, up
  controls.moveTo(1,1,true,0); // opposite corner, down, 180 degrees rotated from the other
  controls.moveTo(1,2,true,0);
  controls.moveTo(1,2,false,0);
  controls.moveTo(2,1,false,0);
  controls.moveTo(2,1,true,0);
  controls.moveTo(2,3,true,0);
  controls.moveTo(2,3,false,0);
  controls.moveTo(1,2,false,0);
  controls.moveTo(1,2,true,0);
  controls.moveTo(1,4,true,0);
  controls.moveTo(1,4,false,0);
  controls.moveTo(2,3,false,0);
  controls.moveTo(2,3,true,0);
  controls.moveTo(2,10,true,20);
  controls.moveTo(10,10,true,0);
  controls.moveTo(10,10,false,0);
  
  // 
  Serial.println("Press enter to continue...");
  while (Serial.available() == 0);
  Serial.readStringUntil('\n');
}
