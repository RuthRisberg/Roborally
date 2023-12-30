#include "Controls.h"

void Controls::init()
{
    motors.init();
}

void Controls::home()
{
    motors.home();
}

void Controls::moveTo(int x, int y, bool up, int angle)
{
    motors.moveTo(xpos(x), ypos(y), up ? uppos : downpos, angle*anglemult);
}

void Controls::moveRobotTo(Robot *robot, int x, int y, int angle)
{
    int newangle = (angle == -1) ? robot->facing : angle;
    moveTo(robot->x, robot->y, downpos, robot->facing);
    moveTo(robot->x, robot->y, uppos, robot->facing);
    moveTo(x, y, uppos, newangle);
    moveTo(x, y, downpos, newangle);
    // Serial.print("moved robot from ");
    // Serial.print(robot->x);
    // Serial.print(",");
    // Serial.print(robot->y);
    // Serial.print(",");
    // Serial.print(robot->facing);
    // Serial.print(" to ");
    // Serial.print(x);
    // Serial.print(",");
    // Serial.print(y);
    // Serial.print(",");
    // Serial.println(angle);
    robot->x = x;
    robot->y = y;
    robot->facing = newangle;
}

void Controls::moveRobot(Robot *robot, int dx, int dy, int dangle)
{
    moveRobotTo(robot, robot->x+dx, robot->y+dy, (robot->facing+dangle+4)%4); // +4 to avoid negative numbers
}

float Controls::xpos(int x)
{
    if (1 <= x && x <= 12) // within first board
    {
        return xref[0] + (x-1) * squaresize;
    }
    else if (13 <= x && x <= 24) // within second board
    {
        Serial.println("WARNING: Trying to move to other board");
        return xref[1] + (x-13) * squaresize;
    }
    else // outside
    {
        Serial.println("WARNING: Trying to move off the board");
        return xref[0];
    }
}

float Controls::ypos(int y)
{
    if (1 <= y && y <= 12) // within first board
    {
        return yref[0] + (y-1) * squaresize;
    }
    else if (y == 13) //within death row (would be second board)
    {
        return yref[1];
    }
    else if (13 <= y && y <= 24) // within second board
    {
        Serial.println("WARNING: Trying to move to other board");
        return yref[1] + (y-13) * squaresize;
    }
    else // outside
    {
        Serial.println("WARNING: Trying to move off the board");
    }
    return yref[0];
}
