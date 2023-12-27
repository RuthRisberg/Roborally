#ifndef Controls_h
#define Controls_h

#include "Robot.h"
#include "Utility.h"

#ifdef cppCompat_h
#include "cpp_test/Motors.h"
#else
#include "src/Motors.h"
#endif


class Controls {
  public:
    Controls() {Serial.println("Controls constructor");}
    void init();
    void home();
    void moveTo(int x, int y, bool up, int angle); // board tile, up/down and angle as 0-3
    void moveRobotTo(Robot *robot, int x, int y, int angle); // angle == -1 for unchanged, moves in a straight line
    void moveRobot(Robot *robot, int dx, int dy, int dangle);

    bool hasRealHoles; // updated by board.setBoard()

  private:
    Motors motors;
    float squaresize = 2025;
    float xref[2] = {1100, 1100+2025*12};
    float yref[2] = {1500, 1500+2025*12};
    float uppos = -1850;
    float downpos = 0;
    float anglemult = 800;

    float xpos(int x);
    float ypos(int y);
};

#endif