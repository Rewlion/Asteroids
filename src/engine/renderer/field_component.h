#pragma once

#include <engine/ecs/BaseComponent.h>
#include <math/math.hpp>

struct FieldComponent : public BaseComponent
{
  Math::fVec2 gameFieldSize;
};
