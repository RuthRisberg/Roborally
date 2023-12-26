#ifndef action_h
#define action_h

#include "Utility.h"
#include "Robot.h"

// actions are one-time-use! when a CompositeAction is executed, it will be destroyed!
class Action {
  public:
    Action() {}

    bool isComposite = false;
    Action *composite1;
    Action *composite2;
    bool powerMove = true;
    int moveabs = -1;
    int moverel = -1;
    int rotation = 0;
    int damage = 0;
    bool death = false;
    int flag = -1;
    bool setspawn = false;

  private:
};

// should take a positive integer, creates a CompositeAction of RelativeMoveActions
class MoveForwardAction : public Action {
  public:
    MoveForwardAction(int steps);
  
  private:
};

class CompositeAction : public Action {
  public:
    CompositeAction(Action *action1, Action *action2);

  private:
};

class AbsoluteMoveAction : public Action {
  public:
    AbsoluteMoveAction(int direction, bool powerMove);

  private:
};

class RelativeMoveAction : public Action {
  public:
    RelativeMoveAction (int direction);

  private:
};

class RotateAction : public Action {
  public:
    RotateAction (int rotation);

  private:
};

class DamageAction : public Action {
  public:
    DamageAction (int damage);

  private:
};

class FlagAction : public Action {
  public:
    FlagAction (int flagNumber);

  private:
};

class UpgradeAction : public Action {
  public:
    UpgradeAction () {}

  private:
};

class SaveAction : public Action {
  public:
    SaveAction ();

  private:
};

class NothingAction : public Action {
  public:
    NothingAction() {}

  private:
};

class DeathAction : public Action {
  public:
    DeathAction();

  private:
};

#endif