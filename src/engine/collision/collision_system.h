#pragma once

#include <engine/ecs/BaseSystems.h>

class Group;

class CollisionSystem : public LogicSystem
{
public:
  CollisionSystem(Context* ecsContext);

  virtual void Update(const float dt) override;

private:
  Group* m_CircleComponentGroup;
};