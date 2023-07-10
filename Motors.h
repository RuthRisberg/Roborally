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
    void move_steppers();
    void disable_outputs();
    bool home();
    void move(float d1, float d2);
    
    uint32_t xbutton = 11;
    uint32_t ybutton = 10;
    uint16_t pins[6][4] = { { 2,  5,  4,  3}, // first movement motor
                            { 6,  9,  8,  7}, // second movement motor
                            {46, 49, 48, 47}, // spin motor
                            {22, 25, 24, 23},
                            {36, 39, 38, 37},
                            {40, 43, 42, 41}};
    int stall_values[3] = {21, 21, 21};
    int microstep[3] = {16, 16, 16};
    int current[3] = {1200, 1200, 1200};
    int testSpeed[3] = {1000, 1000, 1000};
    TMC2130Stepper driver(uint16_t* motorpins);
    AccelStepper stepper(uint16_t* motorpins);
    TMC2130Stepper drivers[3] = {driver(pins[0]), driver(pins[1]), driver(pins[2])};
    AccelStepper steppers[3]  = {stepper(pins[0]), stepper(pins[1]), stepper(pins[2])};
    
    
  private:
    
};

#endif
