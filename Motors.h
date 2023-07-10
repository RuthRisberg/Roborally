#ifndef Motors_h
#define Motors_h

#include "Arduino.h"
#include <TMC2130Stepper.h>
#include <TMC2130Stepper_REGDEFS.h>
#include <AccelStepper.h>

class Motors{
  
  public:
    Motors();
    void init();
    void move_steppers(int stats);
    void disable_outputs();
    void homeMotor(int motor, bool dir);
    void move(float d1, float d2);
    
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
    TMC2130Stepper driver(uint16_t* motorpins);
    AccelStepper stepper(uint16_t* motorpins);
    TMC2130Stepper drivers[6] = {driver(pins[0]), driver(pins[1]), driver(pins[2]), driver(pins[3]), driver(pins[4]), driver(pins[5])};
    AccelStepper steppers[6]  = {stepper(pins[0]), stepper(pins[1]), stepper(pins[2]), stepper(pins[3]), stepper(pins[4]), stepper(pins[5])};
    
    
  private:
    
};

#endif
