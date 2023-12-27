#include <iostream>
#include "Motors.h"

void Motors::disable_outputs()
{
    outputsOn = false;
}

bool Motors::home()
{
    motor1pos = 0;
    motor2pos = 0;
    motor3pos = 0;
    motor4pos = 0;
    return true;
}

void Motors::moveTo(float x, float y, float up, float angle)
{
    motor1pos = x;
    motor2pos = y;
    motor3pos = up;
    motor4pos = angle;
    //std::cout << x << ' ' << y << ' ' << up << ' ' << angle << '\n';
}
