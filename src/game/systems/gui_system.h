#pragma once

#include <engine/ecs/BaseSystems.h>

#include <vector>

class Context;
class Group;
class Entity;

class GuiSystem : public LogicSystem
{
public:
  GuiSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  void ReconstructPlayerLifesWidget(const unsigned int newLifesCount);

private:
  Group* m_GameStatisticsGroup;
  Group* m_GameFieldGroup;
  Group* m_UpdateScoreEventsGroup;
  Group* m_UpdatePlayerLifesEventsGroup;

  Entity* m_ScoreWidget;
  Entity* m_LoseWidget;
  std::vector<Entity*> m_PlayerLifesWidget;
};
