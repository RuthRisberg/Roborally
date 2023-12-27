#ifndef RoboRallyUtility_h
#define RoboRallyUtility_h

#ifndef ARDUINO_SAM_DUE
#include "cpp_test/cppCompat.h" // remove for Arduino c
#else
#include "Arduino.h"
#endif

#ifndef cppCompat_h
int stringToInt(String s);
#endif

namespace utility
{

  int dx(int dir);
  int dy(int dir);
};

namespace move 
{
  enum moveOutcome {
    edge,
    death,
    fail,   // other is -1 for wall, index of robot hit otherwise
    push,   // other is index of robot to push (need to implement pushing robots)
    success
  };
};

namespace tiletype
{
  enum tiletype {
    conveyor,
    expressConveyor,
    conveyorEntry, // only used for curved conveyors
    wall,
    hole,
    laser,
    wrench,
    wrenchAndHammer,
    pusher,
    gear,
    flag,
    edge
  };
};

struct tilemodifier {
  tiletype::tiletype type;
  int modifier;
};





#endif