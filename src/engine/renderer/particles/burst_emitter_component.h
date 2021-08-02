#pragma once

#include <engine/ecs/BaseComponent.h>
#include <engine/renderer/strip_lines_component.h>

struct BurstParticlesEmitterComponent: public SceneComponent
{
  std::vector<StripLinesComponent::Point> particleModel;
  size_t nParticlesToSpawn = 0;
  int minParticleVelocity = 0;
  int maxParticleVelocity = 0;
  Math::fVec2 particleInitialSize = { 0.0f, 0.0f };
  float particleSecondsToLive = 0.0f;
};
