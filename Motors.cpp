#include "Arduino.h"
#include "Motors.h"
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include <AccelStepper.h>


#define RED_PIN 10
#define GREEN_PIN 11
#define BLUE_PIN 12
#define YELLOW_PIN 13

//#define GB_STALL_VALUE 23 //higher stall value setting -> higher stallvalue output
//#define RO_STALL_VALUE 22
//#define CLAW_STALL_VALUE 18
#define GB_STOP_OUT 650  //higher value -> stop sooner
#define RO_STOP_OUT 650  //lower value -> stop later
#define GB_STOP_IN 470
#define RO_STOP_IN 500
#define CLAW_STOP_VALUE 800

#define MOVE_HOME_SPEED 1200
#define MOVE_NORMAL_SPEED 2000


Motors::Motors(uint16_t *pin){
  for(int a=0; a<6; a++){
    for(int b=0; b<4; b++){
      pins[a][b] = *(pin + 4*a + b);
    }
  }
  TMC2130Stepper drivers[6] = {driver(0), driver(1), driver(2), driver(3), driver(4), driver(5)};
  AccelStepper steppers[6]  = {stepper(0), stepper(1), stepper(2), stepper(3), stepper(4), stepper(5)};
}

TMC2130Stepper Motors::driver(int number){
  return(TMC2130Stepper(pins[number][0], pins[number][1], pins[number][2], pins[number][3]));
}
AccelStepper Motors::stepper(int number){
  return(AccelStepper(steppers[number].DRIVER, pins[number][2], pins[number][1]));
}

void Motors::init(){
  for(int i=0; i<6; i++){
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
    /*if(i==2){
      drivers[i].sg_stall_value(RO_STALL_VALUE);
    }
    else if(i==5){
      drivers[i].sg_stall_value(GB_STALL_VALUE);
    }
    else{
      drivers[i].sg_stall_value(CLAW_STALL_VALUE);
    }*/

    steppers[i].setEnablePin(pins[i][0]);
    steppers[i].setPinsInverted(false, false, true);
  }
  Serial.println("Motors initialized");
}

void Motors::move_steppers(int stats) {
  Serial.println("Steppers moving");
  int gb = 0;
  int ro = 0;
  bool is_speed = false;
  while(steppers[0].distanceToGo() != 0 || steppers[1].distanceToGo() != 0 || steppers[2].distanceToGo() != 0 || steppers[3].distanceToGo() != 0 || steppers[4].distanceToGo() != 0 || steppers[5].distanceToGo() != 0)
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
    for(int i=0; i<6; i++){
      steppers[i].run();
    }
  }
  Serial.println("Steppers moved");
}

void Motors::disable_outputs(){
  for(int i=0; i<6; i++){
    steppers[i].disableOutputs();
  }
  Serial.println("Outputs disabled");
}

void Motors::homeMotor(int motor, bool dir){                   //dir=0 -> home out, dir=1 -> home in
  if(motor==2||motor==5){
    steppers[motor].enableOutputs();
    steppers[motor].setMaxSpeed((float)MOVE_HOME_SPEED);
    steppers[motor].setAcceleration((float)8000);
    if(dir==0){
      steppers[motor].move((float)-10000);
    }
    else if(dir==1){
      steppers[motor].move((float)10000);
    }
    move_steppers(1);
    steppers[motor].setCurrentPosition((long)0);
  }
  else{
    Serial.println("Only RO and GB motors are homeable");
  }
  Serial.println("Motors homed");
}

void Motors::move(float d1, float d2){
  float distance[2] = {d1,d2};
  
  for(int i=0; i<2; i++){ //runs twice, for i=2 and i=5
    steppers[i].enableOutputs();
  
    steppers[i].setMaxSpeed((float)MOVE_NORMAL_SPEED);
  
    steppers[i].setAcceleration((float)8000);
  
    steppers[i].move((float)1600 * distance[i]);
  }
  
  move_steppers(0);

  for(int i=0; i<2; i++) if(distance[i]<0) steppers[i].disableOutputs();
}

