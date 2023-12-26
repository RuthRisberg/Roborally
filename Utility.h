#ifndef RoboRallyUtility_h
#define RoboRallyUtility_h

#include "Arduino.h"

namespace utility
{
  int dx(int dir);
  int dy(int dir);
};

namespace move 
{
  enum moveOutcome {
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