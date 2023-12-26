#include "Arduino.h"
#include "Motors.h"
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include <AccelStepper.h>

#define MOVE_HOME_SPEED 1200
#define MOVE_NORMAL_SPEED 4000


Motors::Motors(){
}

TMC2130Stepper Motors::driver(uint16_t* motorpins){
  return(TMC2130Stepper(motorpins[0], motorpins[1], motorpins[2], motorpins[3]));
}
AccelStepper Motors::stepper(uint16_t* motorpins){
  return(AccelStepper(AccelStepper::DRIVER, motorpins[2], motorpins[1]));
}

void Motors::init(){
  for(int i=0; i<4; i++){
    pinMode(pins[i][0], OUTPUT);
    digitalWrite(pins[i][0], HIGH);
    drivers[i].begin();             // Initiate pins and registeries
    drivers[i].setCurrent(current[i], 0.11, 0.5);
    //drivers[i].rms_current(current[i]);    // Set stepper current to X mA. The command is the same as command TMC2130.setCurrent(X, 0.11, 0.5);
    drivers[i].stealthChop(1);      // Enable extremely quiet stepping
    drivers[i].stealth_autoscale(1);
    drivers[i].microsteps(microstep[i]);
    drivers[i].diag1_stall(1);
    drivers[i].diag1_active_high(1);
    drivers[i].coolstep_min_speed(0xFFFFF); // 20bit max
    drivers[i].THIGH(0);
    drivers[i].semin(5);
    drivers[i].semax(2);
    drivers[i].sedn(0b01);
    drivers[i].sg_stall_value(stall_values[i]);
    steppers[i].setEnablePin(pins[i][0]);
    steppers[i].setPinsInverted(false, false, true);
    steppers[i].setMaxSpeed((float)MOVE_NORMAL_SPEED);
    steppers[i].setAcceleration((float)4000);
  }
  Serial.println("Motors initialized");

  pinMode(xbutton, INPUT_PULLUP);
  pinMode(ybutton, INPUT_PULLUP);
  pinMode(stopbutton, INPUT_PULLUP);
  Serial.println("Buttons initialized");
}

// execute the movement that has been defined in moveTo
void Motors::move_steppers() {
  Serial.println("Steppers moving");

  while(steppers[0].distanceToGo() != 0 || steppers[1].distanceToGo() != 0 || steppers[2].distanceToGo() != 0 || steppers[3].distanceToGo() != 0)
  {
    if(digitalRead(stopbutton)==0)
    {
      disable_outputs();
      Serial.println("STOP BUTTON PRESSED");
      while(digitalRead(stopbutton)==0);
      delay(500);
      while(digitalRead(stopbutton)==1);
      delay(500);
      for(int i=0; i<4; i++){
        if(steppers[i].distanceToGo()!=0)  steppers[i].enableOutputs();
      }
    }
    for(int i=0; i<4; i++){
      steppers[i].run();
    }
  }
  Serial.println("Steppers moved");
}

void Motors::disable_outputs(){
  for(int i=0; i<4; i++){
    steppers[i].disableOutputs();
  }
  Serial.println("Outputs disabled");
}

// home the CoreXY
// todo: implement homing of the two other motors
bool Motors::home(){
  // setup
  for (int i=0; i<2; i++) {
    steppers[i].enableOutputs(); // turn on motors
    steppers[i].setMaxSpeed((float)MOVE_HOME_SPEED); // homing speed
  }

  Serial.println("homing setup done");

  // x
  steppers[0].move(-30000); // start moving (the motors should be stopped before moving this full distance)
  steppers[1].move(-30000);
  while (digitalRead(xbutton) == 0) {
    // run() needs to continuously be called to make the motors run
    steppers[0].run();
    steppers[1].run();

    // if the motors move the full distance without hitting the endpoint sensor
    if (steppers[0].distanceToGo() == 0 and steppers[1].distanceToGo() == 0) {
      steppers[0].setMaxSpeed(MOVE_NORMAL_SPEED);
      steppers[1].setMaxSpeed(MOVE_NORMAL_SPEED);
      disable_outputs();
      return (false);
    }
  }

  Serial.println("homing x done");

  // y (similar to x)
  steppers[0].move(-30000);
  steppers[1].move(30000);
  while (digitalRead(ybutton) == 0) {
    steppers[0].run();
    steppers[1].run();

    if (steppers[0].distanceToGo() == 0 and steppers[1].distanceToGo() == 0) {
      steppers[0].setMaxSpeed(MOVE_NORMAL_SPEED);
      steppers[1].setMaxSpeed(MOVE_NORMAL_SPEED);
      disable_outputs();
      return (false);
    }
  }

  Serial.println("homing y done");

  // teardown
  steppers[0].setMaxSpeed(MOVE_NORMAL_SPEED); // set the speed back to normal
  steppers[1].setMaxSpeed(MOVE_NORMAL_SPEED);
  steppers[0].setCurrentPosition((long)0); // mark the current position as position 0
  steppers[1].setCurrentPosition((long)0);
  steppers[2].setCurrentPosition((long)0);
  steppers[3].setCurrentPosition((long)0);
  disable_outputs(); // turn off/relax motors

  Serial.println("Motors homed");
  return (true);
}

void Motors::moveTo(float x, float y, float up, float angle){
  
  steppers[0].enableOutputs();
  steppers[1].enableOutputs();
  steppers[0].moveTo(x+y);
  steppers[1].moveTo(x-y);

  steppers[2].enableOutputs();
  steppers[2].moveTo(up);

  steppers[3].enableOutputs();
  steppers[3].moveTo(angle);

  // debug prints
  // for (int i = 0; i < 4; i++) Serial.print(steppers[i].distanceToGo());
  // Serial.println();
  
  move_steppers(); // execute the movement

  // for (int i = 0; i < 4; i++) Serial.print(steppers[i].distanceToGo());
  // Serial.println();

  for(int i=0; i<4; i++) steppers[i].disableOutputs();
}

