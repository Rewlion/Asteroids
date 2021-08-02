#pragma once

#include <engine/ecs/BaseComponent.h>

struct DeathClockComponent : public BaseComponent
{
  float timeToLive = 0.0f;
  float tickedTime = 0.0f;
};
