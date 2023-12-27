#include "Action.h"



CompositeAction::CompositeAction(Action *action1, Action *action2)
{
    // Serial.println("composite");
    isComposite = true;
    composite1 = action1;
    composite2 = action2;
}

AbsoluteMoveAction::AbsoluteMoveAction(int8_t direction, bool powerMove)
{
    // Serial.println("absolute");
    moveabs = direction;
    this->powerMove = powerMove;
}

RelativeMoveAction::RelativeMoveAction(int8_t direction)
{
    // Serial.println("relative");
    moverel = direction;
}

RotateAction::RotateAction(int8_t rotation)
{
    // Serial.println("rotate");
    this->rotation = rotation;
}

DamageAction::DamageAction(int8_t damage)
{
    // Serial.println("damage");
    this->damage = damage;
}

FlagAction::FlagAction(int8_t flagNumber)
{
    // Serial.println("flag");
    flag = flagNumber;
}

SaveAction::SaveAction()
{
    // Serial.println("save");
    setspawn = true;
}

DeathAction::DeathAction()
{
    // Serial.println("death");
    death = true;
}

MoveForwardAction::MoveForwardAction(int8_t steps)
{
    // Serial.println("composite");
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
