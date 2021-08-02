#pragma once

#include <engine/ecs/BaseComponent.h>
#include <engine/renderer/strip_lines_component.h>

struct DirectParticleEmitterComponent : public SceneComponent
{
  float secondsToEmmit = 0.0f;
  float secondsSinceLastEmmit = 0.0f;
  std::vector<StripLinesComponent::Point> particleModel;
  float particleSecondsToLive = 1.0f;
  Math::fVec2 particleInitialSize = { 1.0f, 1.0f };
  float particleSizeDecreaseSpeed = 0.0f;
  float particleVelocity = 0.0f;
  bool isInitialRotationRandomized = false;
  bool isEnabled = false;
};
