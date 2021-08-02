#pragma once

#include <engine/ecs/BaseSystems.h>

class Group;

class DeathClockSystem : public LogicSystem
{
public:
  DeathClockSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  Group* m_DeathClockGroup;
};