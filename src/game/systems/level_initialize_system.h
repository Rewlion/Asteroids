#pragma once

#include <engine/ecs/BaseSystems.h>

class LevelInitializeSystem : public InitializationSystem
{
public:
  LevelInitializeSystem(Context* ecsContext);

  virtual void Initialize() override;

private:

};