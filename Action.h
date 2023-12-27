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
    int8_t moveabs = -1;
    int8_t moverel = -1;
    int8_t rotation = 0;
    int8_t damage = 0;
    bool death = false;
    int8_t flag = -1;
    bool setspawn = false;

  private:
};

// should take a positive integer, creates a CompositeAction of RelativeMoveActions
class MoveForwardAction : public Action {
  public:
    MoveForwardAction(int8_t steps);
  
  private:
};

class CompositeAction : public Action {
  public:
    CompositeAction(Action *action1, Action *action2);

  private:
};

class AbsoluteMoveAction : public Action {
  public:
    AbsoluteMoveAction(int8_t direction, bool powerMove);

  private:
};

class RelativeMoveAction : public Action {
  public:
    RelativeMoveAction (int8_t direction);

  private:
};

class RotateAction : public Action {
  public:
    RotateAction (int8_t rotation);

  private:
};

class DamageAction : public Action {
  public:
    DamageAction (int8_t damage);

  private:
};

class FlagAction : public Action {
  public:
    FlagAction (int8_t flagNumber);

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