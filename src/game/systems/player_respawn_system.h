#pragma once

#include <engine/ecs/BaseSystems.h>

#include <optional>

class Group;
class Context;

class PlayerRespawnSystem : public LogicSystem
{
public:
  PlayerRespawnSystem(Context* ecsContext);

  virtual void Update(const float dt) override;

private:
  Group* m_PlayerGroup;
  Group* m_GameStatisticsGroup;
  Group* m_GameFieldGroup;
  bool m_TimerToSpawnPlayerAlreadySetup = false;
};
