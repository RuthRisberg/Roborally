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
    void disable_outputs();
    bool home();
    void moveTo(float x, float y, float up, float angle);
    
    
  private:
    void move_steppers();
    uint32_t xbutton = 11;
    uint32_t ybutton = 10;
    uint32_t stopbutton = 13;
    uint16_t pins[6][4] = { { 2,  5,  4,  3}, // first CoreXY motor
                            { 6,  9,  8,  7}, // second CoreXY motor
                            {46, 49, 48, 47}, // up/down motor
                            {22, 25, 24, 23}, // spin motor
                            {36, 39, 38, 37}, // 
                            {40, 43, 42, 41}};// 
    int stall_values[4] = {21, 21, 21, 21};
    int microstep[4] = {16, 16, 16, 16};
    int current[4] = {1200, 1200, 1200, 1200};
    int testSpeed[4] = {1000, 1000, 1000, 1000};
    TMC2130Stepper driver(uint16_t* motorpins);
    AccelStepper stepper(uint16_t* motorpins);
    TMC2130Stepper drivers[4] = {driver(pins[0]),  driver(pins[1]),  driver(pins[2]),  driver(pins[3]) };
    AccelStepper steppers[4]  = {stepper(pins[0]), stepper(pins[1]), stepper(pins[2]), stepper(pins[3])};
    
};

#endif
