#pragma once

#include <engine/ecs/BaseSystems.h>

class Group;
class Context;
class SpawnRequestComponent;

class GameObjectsSpawnSystem : public LogicSystem
{
public:
  GameObjectsSpawnSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  void Spawn(const SpawnRequestComponent* request);

private:
  Group* m_SpawnRequestGroup;
};
