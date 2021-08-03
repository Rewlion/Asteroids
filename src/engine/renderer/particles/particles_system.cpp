#include "particles_system.h"
#include "direct_emitter_component.h"
#include "burst_emitter_component.h"

#include <engine/ecs/Context.h>
#include <game/components/death_clock_component.h>
#include <game/components/movement_component.h>

namespace Render
{
  ParticlesSystem::ParticlesSystem(Context* ecsContext)
    : LogicSystem(ecsContext)
    , m_DirectEmitterGroup(ecsContext->GetGroup<DirectParticleEmitterComponent>())
    , m_BurstEmitterGroup(ecsContext->GetGroup< BurstParticlesEmitterComponent>())
  {

  }

  void ParticlesSystem::Update(const float dt)
  {
    UpdateDirectEmitters(dt);
    UpdateBurstEmitters(dt);
  }

  void ParticlesSystem::UpdateDirectEmitters(const float dt)
  {
    for(Entity* e: m_DirectEmitterGroup->GetEntities())
    if (e)
    for(DirectParticleEmitterComponent* emitter: e->GetComponents<DirectParticleEmitterComponent>())
    {
      emitter->secondsSinceLastEmmit += dt;
      if (emitter->isEnabled && (emitter->secondsSinceLastEmmit > emitter->secondsToEmmit))
      {
        const Math::fVec2 particlePosition = emitter->GetWorldLocation();
        const float particleRotation = emitter->GetWorldRotation();

        auto* particle = pContext->GetEntityManager()->NewEntity();
        auto* root = particle->AddComponent<SceneComponent>("root");
        root->Detach();
        root->SetLocalLocation(particlePosition);
        root->SetLocalRotation(particleRotation);
        root->SetLocalSize(emitter->particleInitialSize);

        auto* model = particle->AddComponent<StripLinesComponent>("particle model");
        const float modelRotation = emitter->isInitialRotationRandomized ? static_cast<float>(std::rand() % 120) : 0.0f;
        model->AttachTo(root);
        model->SetPoints(emitter->particleModel);
        model->SetLocalLocation({ 0.0f, 0.0f });
        model->SetLocalRotation(modelRotation);
        model->SetColor(Color::Bisque);
        model->SetLocalSize({ 1.0f, 1.0f });

        auto* deathTimer = particle->AddComponent<DeathClockComponent>("time before particle dies");
        deathTimer->tickedTime = 0.0f;
        deathTimer->timeToLive = emitter->particleSecondsToLive;

        auto* movement = particle->AddComponent<ConstantMovementComponent>("particle movement");
        movement->velocity = root->GetForward() * emitter->particleVelocity;

        emitter->secondsSinceLastEmmit = 0.0f;
      }
    }
  }

  void ParticlesSystem::UpdateBurstEmitters(const float dt)
  {
    for(Entity* e: m_BurstEmitterGroup->GetEntities())
    if (e)
    for(BurstParticlesEmitterComponent* emitter: e->GetComponents<BurstParticlesEmitterComponent>())
    {
      float anglePerParticle = 360.0f / emitter->nParticlesToSpawn;
      for (size_t i = 0; i < emitter->nParticlesToSpawn; ++i)
      {
        const Math::fVec2 particlePosition = emitter->GetWorldLocation();
        const float particleRotation = anglePerParticle * i;

        auto* particle = pContext->GetEntityManager()->NewEntity();
        auto* root = particle->AddComponent<SceneComponent>("root");
        root->Detach();
        root->SetLocalLocation(particlePosition);
        root->SetLocalRotation(particleRotation);
        root->SetLocalSize(emitter->particleInitialSize);

        auto* model = particle->AddComponent<StripLinesComponent>("particle model");
        model->AttachTo(root);
        model->SetPoints(emitter->particleModel);
        model->SetLocalLocation({ 0.0f, 0.0f });
        model->SetLocalRotation(0.0f);
        model->SetColor(Color::Bisque);
        model->SetLocalSize({ 1.0f, 1.0f });

        auto* deathTimer = particle->AddComponent<DeathClockComponent>("time before particle dies");
        deathTimer->tickedTime = 0.0f;
        deathTimer->timeToLive = emitter->particleSecondsToLive;

        auto* movement = particle->AddComponent<ConstantMovementComponent>("particle movement");
        const float velocity = Math::GetRandom<float>(emitter->minParticleVelocity, emitter->maxParticleVelocity);
        movement->velocity = root->GetForward() * velocity;
      }

      pContext->GetEntityManager()->ReleaseEntity(e);
    }
  }
}