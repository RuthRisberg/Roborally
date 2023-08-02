/*
todo:
change die() to being called at the end of each phase
how quickly are dead robots removed? - currently they stay the full laser-phase, including shooting their own lasers
respawning
asking for power down
getting removed when dead
conveyor entries
check how many steps to move for x, y, up/down and spin
test rotation, pushers, conveyors, lasers
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
