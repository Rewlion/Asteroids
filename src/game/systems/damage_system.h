#pragma once

#include <engine/ecs/BaseSystems.h>

class Context;
class Group;
class Entity;
struct SceneComponent;

class DamageSystem : public LogicSystem
{
public:
  DamageSystem(Context* ecs);

  virtual void Update(const double dt) override;

private:
  void OnDeath(Entity* e);

private:
  Group* m_EntityWithCollisionGroup;
  Group* m_GameStatisticsGroup;
};