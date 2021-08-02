#pragma once

#include <engine/ecs/BaseSystems.h>
#include <engine/ecs/Group.h>

class MovementSystem : public LogicSystem
{
public:
  MovementSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  Group* m_MovementGroup;
  Group* m_ConstantMovementGroup;
};
