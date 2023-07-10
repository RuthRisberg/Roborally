#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include <AccelStepper.h>

class Motors{
  
  public:
    Motors(uint16_t *pin); // [6][4] containing [motors R,O,RO,G,B,GB][pins]
    void init();
    void move_steppers(int stats);
    void disable_outputs();
    void homeMotor(int motor, bool dir);
    void rotations(int R, int O, int G, int B);
    void moves(float RO, float GB);
    void ROturns(int R, int O, int RO, int wait);
    void GBturns(int G, int B, int GB, int wait);
    void turn_series(int turns[][2], int len);
    void edit_stallvalue(int motor, int increase);
    void edit_microstep(int motor, int increase);
    void edit_current(int motor, int increase);
    void edit_testSpeed(int motor, int increase);
    
    // no this is not how things should be, but it has to be like this now becuse TMC2130Stepper won't accept declaration without initialization
    uint16_t pins[6][4] = { { 2,  5,  4,  3},
                            { 6,  9,  8,  7},
                            {46, 49, 48, 47},
                            {22, 25, 24, 23},
                            {36, 39, 38, 37},
                            {40, 43, 42, 41}};
    int stall_values[6] = {21, 21, 21, 21, 21, 21};
    int microstep[6] = {16, 16, 16, 16, 16, 16};
    int current[6] = {1200, 1200, 1200, 1200, 1200, 1200};
    int testSpeed[6] = {1000, 1000, 1000, 1000, 1000, 1000};
    TMC2130Stepper driver(int number);
    AccelStepper stepper(int number);
    TMC2130Stepper drivers[6] = {driver(0), driver(1), driver(2), driver(3), driver(4), driver(5)};
    AccelStepper steppers[6]  = {stepper(0), stepper(1), stepper(2), stepper(3), stepper(4), stepper(5)};
    
    
  private:
    
};

#endif
