#pragma once

#include <engine/ecs/BaseSystems.h>

#include <optional>

class Group;
class Context;

class AsteroidsSpawnSystem : public LogicSystem
{
public:
  AsteroidsSpawnSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  Group* m_IdentityGroup;
  Group* m_GameFieldGroup;
  bool m_TimerToSpawnAsteroidsAlreadySetup = false;
};
