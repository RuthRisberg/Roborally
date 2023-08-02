/*
todo:
robots shooting lasers
respawning
asking for power down
getting removed when dead
conveyor entries
check how many steps to move for x, y, up/down and spin
double check rotation, pushers, conveyors
*/

#include "Game.h"

Game game;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Hello world!");

  game.init();
  game.runGame();

  Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  // while (Serial.available() == 0);
  // String str = Serial.readStringUntil('\n');
  // Serial.println(str);
}
