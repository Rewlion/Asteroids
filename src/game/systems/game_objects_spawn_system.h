#pragma once

#include <engine/ecs/BaseSystems.h>

class Group;
class Context;
struct SpawnRequestComponent;

class GameObjectsSpawnSystem : public LogicSystem
{
public:
  GameObjectsSpawnSystem(Context* ecsContext);

  virtual void Update(const float dt) override;

private:
  void Spawn(const SpawnRequestComponent* request);

private:
  Group* m_SpawnRequestGroup;
};
