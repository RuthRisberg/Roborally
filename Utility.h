#ifndef RoboRallyUtility_h
#define RoboRallyUtility_h

class Action {
  public:
    enum actiontype {
      damage,   // amount
      die,      // -
      flag,     // nr
      moveabs,  // direction (used for conveyors)
      moverel,  // direction, distance (used for cards)
      nothing,  // -
      rotation, // directiondiff
      setspawn, // -
      upgrade   // -
    };
    Action();
    Action(actiontype at){type=at;};
    Action(actiontype at, int v1){type=at; property1=v1;};
    Action(actiontype at, int v1, int v2){type=at; property1=v1; property2=v2;};
    actiontype type;
    int property1=-1;
    int property2=-1;
    actiontype secondaryActiontype; // other action done first, second only done if first succeeds. If second fails, first action is undone
    int property21=-1;
    int property22=-1;
  private:

};

class Move {
  public:
    enum moveOutcomeType {
      death,
      fail,   // other is -1 for wall, index of robot hit otherwise
      success
    };
    struct moveOutcome {
      moveOutcomeType result;
      int other;
    };
};

class TileType {
  public:
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
      flag
    };
    struct tilemodifier {
      tiletype type;
      int modifier;
    };
};

class Card {
  public:
    Card();
    Card(Action a, int n) {action = a; number = n;}
    Action action;
    int number;
};

class Robot {
  public:
    Robot();
    Robot(int _id){id=_id;}
    Card cards[5];
    int id;
    int x=0;
    int y=0;
    int facing=0;
    int lives=3;
    int damage=0;
    int respawnx=0;
    int respawny=0;
    int currentflag=0;
    
};


#endif