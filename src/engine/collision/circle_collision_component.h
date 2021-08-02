#pragma once

#include <engine/ecs/BaseComponent.h>

struct CircleCollisionComponent : public BaseComponent
{
  float radius = 0.0f;
};
