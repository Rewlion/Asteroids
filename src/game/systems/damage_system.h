#pragma once

#include <engine/ecs/BaseSystems.h>
#include <game/components/identity_component.h>

class Context;
class Group;
class Entity;
struct SceneComponent;
struct IdentityComponent;

class DamageSystem : public LogicSystem
{
public:
  DamageSystem(Context* ecs);

  virtual void Update(const float dt) override;

private:
  void OnDeath(Entity* e);
  void OnAsteroidDeath(const IdentityComponent* destroidEntityIdentity, const SceneComponent* destroidEntityRoot);
  void OnPlayerShipDeath(const IdentityComponent* destroidEntityIdentity);
  void SpawnNewAsteroids(const IdentityComponent* destroidAsteroidIdentity, const SceneComponent* destroidAsteroidRoot);
  void SpawnBurstAfterAsteroidDeath(const SceneComponent* destroidAsteroidRoot);
  void PlayAsteroidDeathSound(const IdentityComponent* asteroidIdentity);

private:
  Group* m_EntityWithCollisionGroup;
  Group* m_GameStatisticsGroup;
};