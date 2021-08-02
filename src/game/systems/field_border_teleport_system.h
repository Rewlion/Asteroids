#pragma once

#include <engine/ecs/BaseSystems.h>

class Context;
class Group;

class FieldBorderTeleportSystem : public LogicSystem
{
public:
  FieldBorderTeleportSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  Group* m_ObjectsSceneComponentGroup;
  Group* m_GameFieldGroup;
};