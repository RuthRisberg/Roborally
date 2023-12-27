/*
rules (only selected elementss implemented):
https://www.thealexandrian.net/creations/roborally/roborally-ultimate-collection-rulebook.pdf

*/

#include "Game.h"

// Controls controls;

// #include <malloc.h>
// #include <stdlib.h>
// #include <stdio.h>

// extern char _end;
// extern "C" char *sbrk(int i);
// char *ramstart=(char *)0x20070000;
// char *ramend=(char *)0x20088000;

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
  // controls.init();
  // controls.home();

  // Controls controls;
  // Player players[8];
  // Game game;

  // delay(1000);
  // char *heapend=sbrk(0);
	// register char * stack_ptr asm ("sp");
	// struct mallinfo mi=mallinfo();
	// printf("\nDynamic ram used: %d\n",mi.uordblks);
	// printf("Program static ram used %d\n",&_end - ramstart); 
	// printf("Stack ram used %d\n\n",ramend - stack_ptr); 
	// printf("My guess at free mem: %d\n",stack_ptr - heapend + mi.fordblks);
  // delay(1000);

  // Board board;
  // Input input;
  // int nPlayers;
  // void playCards(int reg);
  // Tile defaultTile;

  Game game;
  Serial.println("game constructor done!");
  game.init();
  game.setNumberOfPlayers(1);
  Serial.println("Run game");
  game.runGame();

  // delay(1000);
  // char *heapend=sbrk(0);
	// register char * stack_ptr asm ("sp");
	// struct mallinfo mi=mallinfo();
	// printf("\nDynamic ram used: %d\n",mi.uordblks);
	// printf("Program static ram used %d\n",&_end - ramstart); 
	// printf("Stack ram used %d\n\n",ramend - stack_ptr); 
	// printf("My guess at free mem: %d\n",stack_ptr - heapend + mi.fordblks);

  Serial.println("Setup done");
}

/*
void move_around() {
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

  controls.moveTo(1,4,false,0);
  controls.moveTo(1,4,true,0);
  controls.moveTo(1,1,true,0);
  controls.moveTo(1,1,false,0);

  controls.moveTo(10,10,false,0);
  controls.moveTo(10,10,true,0);
  controls.moveTo(2,1,true,0);
  controls.moveTo(2,1,false,0);
}
*/

void loop() {
  /*
  // Run only the motors object
  motors.moveTo(0, 0, 1000, 0);
  motors.moveTo(0, 0, 0, 0);
  motors.moveTo(0, 0, 0, 800);
  motors.moveTo(0, 0, 0, 0);
  */

  
  // 
  Serial.println("Press enter to continue...");
  while (Serial.available() == 0);
  Serial.readStringUntil('\n');
}
