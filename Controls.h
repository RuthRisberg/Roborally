#ifndef Controls_h
#define Controls_h

#include "Motors.h"
#include "Robot.h"
#include "Utility.h"

class Controls {
  public:
    Controls();
    void init();
    void home();
    void moveTo(int x, int y, bool up, int angle); // board tile, up/down and angle as 0-3
    void moveRobotTo(Robot *robot, int x, int y, int angle); // angle == -1 for unchanged, moves in a straight line
    void moveRobot(Robot *robot, int dx, int dy, int dangle);

  private:
    Motors motors;

    // todo: get these values right
    float xref[2] = {200};
    float yref[2] = {200};
    float uppos = 0;
    float downpos = 0;
    float anglemult = 800;
    float squaresize = 100;


    float xpos(int x);
    float ypos(int y);
};

#endif