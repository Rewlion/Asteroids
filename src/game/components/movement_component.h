#pragma once

#include <engine/ecs/BaseComponent.h>
#include <math/math.hpp>

struct MovementComponent : public BaseComponent
{
  float rorationMaxVelocity = 0.0f;
  float rotationVelocity = 0.0f;

  float maxAcceleration = 0.0f;
  float acceleration = 0.0f;
  Math::fVec2 velocity = { 0.0f, 0.0f };
};

struct ConstantMovementComponent : public BaseComponent
{
  Math::fVec2 velocity = { 0.0f, 0.0f };
};