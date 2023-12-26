#include "Action.h"



CompositeAction::CompositeAction(Action *action1, Action *action2)
{
    isComposite = true;
    composite1 = action1;
    composite2 = action2;
}

AbsoluteMoveAction::AbsoluteMoveAction(int direction, bool powerMove)
{
    moveabs = direction;
    this->powerMove = powerMove;
}

RelativeMoveAction::RelativeMoveAction(int direction)
{
    moverel = direction;
}

RotateAction::RotateAction(int rotation)
{
    this->rotation = rotation;
}

DamageAction::DamageAction(int damage)
{
    this->damage = damage;
}

FlagAction::FlagAction(int flagNumber)
{
    flag = flagNumber;
}

SaveAction::SaveAction()
{
    setspawn = true;
}

DeathAction::DeathAction()
{
    death = true;
}

MoveForwardAction::MoveForwardAction(int steps)
{
    if (steps <= 0)
    {
        Serial.println("MoveForwardAction created with non-positive argument. Creating NothingAction.");
    }
    else if (steps == 1)
    {
        moverel = 0;
    }
    else
    {
        isComposite = true;
        composite1 = new RelativeMoveAction(0);
        composite2 = new MoveForwardAction(steps-1);
    }
}
