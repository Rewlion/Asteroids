#pragma once

#include <engine/ecs/BaseSystems.h>

class Group;
class Context;

class TimerSystem : public LogicSystem
{
public:
  TimerSystem(Context* ecsContext);

  virtual void Update(const float dt) override;

private:
  Group* m_DoOnceTimerGroup;
};
