#pragma once

#include "identity_component.h"

#include <engine/ecs/BaseComponent.h>
#include <math/math.hpp>

struct SpawnRequestComponent : public BaseComponent
{
  Identity objectType = Identity::None;
  Team team = Team::None;
  Math::fVec2 position = { 0.0f, 0.0f };
  float rotation = 0.0f;
};
