#include "Arduino.h"
#include "Motors.h"
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include <AccelStepper.h>


#define RED_PIN 10
#define GREEN_PIN 11
#define BLUE_PIN 12
#define YELLOW_PIN 13

#define MOVE_HOME_SPEED 1200
#define MOVE_NORMAL_SPEED 2000


Motors::Motors(){
}

TMC2130Stepper Motors::driver(uint16_t* motorpins){
  return(TMC2130Stepper(motorpins[0], motorpins[1], motorpins[2], motorpins[3]));
}
AccelStepper Motors::stepper(uint16_t* motorpins){
  return(AccelStepper(AccelStepper::DRIVER, motorpins[2], motorpins[1]));
}

void Motors::init(){
  for(int i=0; i<3; i++){
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
    steppers[i].setAcceleration((float)8000);
  }
  Serial.println("Motors initialized");

  pinMode(xbutton, INPUT_PULLUP);
  pinMode(ybutton, INPUT_PULLUP);
  Serial.println("Buttons initialized");
}

void Motors::move_steppers() {
  Serial.println("Steppers moving");
  int gb = 0;
  int ro = 0;
  bool is_speed = false;
  while(steppers[0].distanceToGo() != 0 || steppers[1].distanceToGo() != 0 || steppers[2].distanceToGo() != 0)
  {
    /*if(digitalRead(RED_PIN)==0)
    {
      disable_outputs();
      Serial.println("STOP BUTTON PRESSED");
      while(digitalRead(RED_PIN)==0);
      delay(500);
      while(digitalRead(RED_PIN)==1);
      while(digitalRead(RED_PIN)==0);
      for(int i=0; i<6; i++){
        if(steppers[i].distanceToGo()!=0)  steppers[i].enableOutputs();
      }
    }*/
    for(int i=0; i<3; i++){
      steppers[i].run();
    }
  }
  Serial.println("Steppers moved");
}

void Motors::disable_outputs(){
  for(int i=0; i<3; i++){
    steppers[i].disableOutputs();
  }
  Serial.println("Outputs disabled");
}

bool Motors::home(){
  // setup
  for (int i=0; i<2; i++) {
    steppers[i].enableOutputs();
    steppers[i].setMaxSpeed((float)MOVE_HOME_SPEED);
  }

  Serial.println("homing setup done");

  // x
  steppers[0].move(-30000);
  steppers[1].move(-30000);
  while (digitalRead(xbutton) == 0) {
    steppers[0].run();
    steppers[1].run();

    if (steppers[0].distanceToGo() == 0 and steppers[1].distanceToGo() == 0) {
      steppers[0].setMaxSpeed(MOVE_NORMAL_SPEED);
      steppers[1].setMaxSpeed(MOVE_NORMAL_SPEED);
      disable_outputs();
      return (false);
    }
  }

  Serial.println("homing x done");

  // y
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
  steppers[0].setMaxSpeed(MOVE_NORMAL_SPEED);
  steppers[1].setMaxSpeed(MOVE_NORMAL_SPEED);
  steppers[0].setCurrentPosition((long)0);
  steppers[1].setCurrentPosition((long)0);
  disable_outputs();

  Serial.println("Motors homed");
  return (true);
}

void Motors::move(float d1, float d2){
  float distance[2] = {d1,d2};
  
  for(int i=0; i<2; i++){
    steppers[i].enableOutputs();
    steppers[i].move((float)1600 * distance[i]);
  }
  
  move_steppers();

  for(int i=0; i<2; i++) if(distance[i] != 0) steppers[i].disableOutputs();
}

