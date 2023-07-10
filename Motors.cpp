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

void Motors::edit_stallvalue(int motor, int increase) {
  stall_values[motor] += increase;
  drivers[motor].sg_stall_value(stall_values[motor]);
}

void Motors::edit_microstep(int motor, int increase) {
  microstep[motor] += increase;
  drivers[motor].microsteps(microstep[motor]);
}

void Motors::edit_current(int motor, int increase) {
  current[motor] += increase;
  drivers[motor].setCurrent(current[motor], 0.11, 0.5);
  //drivers[motor].rms_current(current[motor]);
}

void Motors::edit_testSpeed(int motor, int increase) {
  testSpeed[motor] += increase;
  steppers[motor].setSpeed(testSpeed[motor]);
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

void Motors::rotations(int R, int O, int G, int B){
  int motor[4] = {0, 1, 3, 4};
  int rotations[4] = {R, O, G, B};
  for(int i=0; i<4; i++){
    steppers[motor[i]].enableOutputs();
  
    steppers[motor[i]].setMaxSpeed((float)1000);
    
    steppers[motor[i]].setAcceleration((float)10000);
    
    steppers[motor[i]].move((float)800 * rotations[i]);
  }
  move_steppers(0);
  for(int i=0; i<4; i++){
    steppers[motor[i]].disableOutputs();
  }
}

void Motors::moves(float RO, float GB){
  float distance[6] = {0, 0, RO, 0, 0, GB};
  
  for(int i=2; i<6; i+=3){ //runs twice, for i=2 and i=5
    steppers[i].enableOutputs();
  
    steppers[i].setMaxSpeed((float)MOVE_NORMAL_SPEED);
  
    steppers[i].setAcceleration((float)8000);
  
    steppers[i].move((float)1600 * distance[i]);
  }
  
  move_steppers(0);

  for(int i=2; i<6; i+=3) if(distance[i]<0) steppers[i].disableOutputs();
}

void Motors::ROturns(int R, int O, int RO, int wait){
  if (RO != 0){
    moves(0, -1);
    delay(wait);
    rotations(RO, -RO, 0, 0);
    delay(wait);
    moves(0, 1);
    delay(wait);
  }
  
  rotations(R, O, 0, 0);
  if((-R-RO)%2!=0 || (-O+RO)%2!=0){
    delay(wait);
    moves(-1, 0);
    delay(wait);
    rotations((-R-RO)%2, (-O+RO)%2, 0, 0);
    delay(wait);
    moves(1, 0);
  }
}
void Motors::GBturns(int G, int B, int GB, int wait){
  if (GB != 0){
    moves(0, -1);
    delay(wait);
    rotations(0, 0, GB, -GB);
    delay(wait);
    moves(0, 1);
    delay(wait);
  }
  
  rotations(0, 0, G, B);
  if((-G-GB)%2!=0 || (-B+GB)%2!=0){
    delay(wait);
    moves(-1, 0);
    delay(wait);
    rotations(0, 0, (-G-GB)%2, (-B+GB)%2);
    delay(wait);
    moves(1, 0);
  }
}

void Motors::turn_series(int turns[][2], int len){
  int a=0;
  int b=0;
  int c=0;
  for (int i=0; i<len; i++){
    int j=turns[i][0];
    int k=turns[i][1];
    
    if(j<=3){
      if(turns[i-1][0]>=4){
        GBturns(a, b, c, 0);
        a=0;
        b=0;
        c=0;
      }
           if(j==1) a=k;
      else if(j==2) b=k;
      else if(j==3) c=k;
    }
    else if(j>=4){
      if(turns[i-1][0]<=3){
        ROturns(a, b, c, 0);
        a=0;
        b=0;
        c=0;
      }
           if(j==4) a=k;
      else if(j==5) b=k;
      else if(j==6) c=k;
    }
  }
  if(turns[len-1][0]<=3){
    ROturns(a, b, c, 0);
  }
  else if(turns[len-1][0]>=4){
    GBturns(a, b, c, 0);
  }
}
