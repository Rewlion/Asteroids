#include "damage_system.h"

#include <engine/collision/entity_collides_component.h>
#include <engine/ecs/Context.h>
#include <engine/scene_component.h>
#include <engine/renderer/particles/burst_emitter_component.h>
#include <engine/sound/utils.h>
#include <game/components/identity_component.h>
#include <game/components/spawn_request_component.h>
#include <game/components/game_statistics_component.h>
#include <game/components/gui_update_event_components.h>
#include <game/game_objects_shapes.h>

#include <set>
#include <math.h>
#include <tuple>

namespace
{
  inline bool IsAsteroid(IdentityComponent* identity)
  {
    const Identity type = identity->identity;
    return type == Identity::BigAsteroid ||
           type == Identity::MediumAsteroid ||
           type == Identity::SmallAsteroid;
  }
}

DamageSystem::DamageSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_EntityWithCollisionGroup(ecsContext->GetGroup<EntityCollidesComponent, IdentityComponent>())
  , m_GameStatisticsGroup(ecsContext->GetGroup<GameStatisticsComponent>())
{
}

void DamageSystem::Update(const float dt)
{
  std::set<Entity*> entitiesToRelease;

  for(Entity* e: m_EntityWithCollisionGroup->GetEntities())
  if (e)
  {
    auto* collisionsEvent = e->GetFirstComponent<EntityCollidesComponent>();
    for(auto* collisionEntity: collisionsEvent->entitiesCollidedWith)
    {
      auto* collisionIdentity = collisionEntity->GetFirstComponent<IdentityComponent>();
      auto* identity = e->GetFirstComponent<IdentityComponent>();

      const bool isAsteroidCollidedWithEnemyBullet = IsAsteroid(identity) &&
        collisionIdentity->identity == Identity::Bullet &&
        collisionIdentity->team != identity->team;

      const bool isOtherEntityCollidedWithEnemy = !IsAsteroid(identity) && (identity->team != collisionIdentity->team);

      const bool isDeathCollision = isAsteroidCollidedWithEnemyBullet || isOtherEntityCollidedWithEnemy;

      if (isDeathCollision)
      {
        OnDeath(e);
        entitiesToRelease.insert(e);
      }
    }
  }

  for (Entity* e : entitiesToRelease)
    pContext->GetEntityManager()->ReleaseEntity(e);
}

void DamageSystem::OnDeath(Entity* e)
{
  auto* destroidEntityRoot = e->GetFirstComponent<SceneComponent>();
  auto* destroidEntityIdentity = e->GetFirstComponent<IdentityComponent>();
  auto* em = pContext->GetEntityManager();

  if (IsAsteroid(destroidEntityIdentity))
  {
    OnAsteroidDeath(destroidEntityIdentity, destroidEntityRoot);
  }
  else if (destroidEntityIdentity->identity == Identity::PlayerShip)
  {
    OnPlayerShipDeath(destroidEntityIdentity);
  }
}

void DamageSystem::OnAsteroidDeath(const IdentityComponent* destroidEntityIdentity, const SceneComponent* destroidEntityRoot)
{
  SpawnNewAsteroids(destroidEntityIdentity, destroidEntityRoot);
  PlayAsteroidDeathSound(destroidEntityIdentity);
  SpawnBurstAfterAsteroidDeath(destroidEntityRoot);

  auto* gameStatistics = m_GameStatisticsGroup->GetFirstNotNullEntity()->GetFirstComponent<GameStatisticsComponent>();
  gameStatistics->playerScore += Game::Utils::DefaultGameObjectsSetttings
                              .at(destroidEntityIdentity->identity)
                              .score;

  auto* guiRequest = pContext->GetEntityManager()->NewEntity()->AddComponent<GuiUpdateScoreComponent>("update score widget event");
  guiRequest->newScore = gameStatistics->playerScore;
}

void DamageSystem::OnPlayerShipDeath(const IdentityComponent* destroidEntityIdentity)
{
  auto* gameStatistics = m_GameStatisticsGroup->GetFirstNotNullEntity()->GetFirstComponent<GameStatisticsComponent>();

  if (gameStatistics->playerLifes > 0)
  {
    gameStatistics->playerLifes -= 1;
    auto* guiRequest = pContext->GetEntityManager()->NewEntity()->AddComponent<GuiUpdatePlayerLifesComponent>("update player life widget event");
    guiRequest->newPlayerLifes = gameStatistics->playerLifes;

    if (gameStatistics->playerLifes == 0)
      gameStatistics->gameStatus = GameStatus::Lose;
  }

  Sound::PlaySound(pContext, L"SmallAsteroidDeathSound");
}

void DamageSystem::SpawnNewAsteroids(const IdentityComponent* destroidAsteroidIdentity, const SceneComponent* destroidAsteroidRoot)
{
  const bool isBigOrMediumAsteroid = destroidAsteroidIdentity->identity == Identity::BigAsteroid
                                  || destroidAsteroidIdentity->identity == Identity::MediumAsteroid;
  
  if (!isBigOrMediumAsteroid)
    return;

  const auto [nAsteroidsToSpawn, asteroidsToSpawnType] = destroidAsteroidIdentity->identity == Identity::BigAsteroid
                                                       ? std::tuple{ 1, Identity::MediumAsteroid }
                                                       : std::tuple{ 2, Identity::SmallAsteroid };

  for (size_t i = 0; i < nAsteroidsToSpawn; ++i)
  {
    auto* request = pContext->GetEntityManager()->NewEntity()->AddComponent<SpawnRequestComponent>("Spawn small asteroid after the death of the medium one");
    request->position = destroidAsteroidRoot->GetLocalLocation();
    request->rotation = destroidAsteroidRoot->GetLocalRotation() + static_cast<float>(std::rand() % 60);
    request->team = destroidAsteroidIdentity->team;
    request->objectType = asteroidsToSpawnType;
  }
}

void DamageSystem::SpawnBurstAfterAsteroidDeath(const SceneComponent* destroidAsteroidRoot)
{
  auto* e = pContext->GetEntityManager()->NewEntity();
  auto* emitter = e->AddComponent<BurstParticlesEmitterComponent>("asteroid's death burst ");
  emitter->SetLocalLocation(destroidAsteroidRoot->GetLocalLocation());
  emitter->SetLocalRotation(0.0f);
  emitter->SetLocalSize({ 1.0f, 1.0f });
  emitter->particleModel = { {-0.5f, -0.5f}, {0.0f, 0.5f}, {0.5f,-0.5f}, {0.0f, -0.5f}, {-0.5f, -0.5f} };
  emitter->nParticlesToSpawn = 20;
  emitter->minParticleVelocity = 10.0f;
  emitter->maxParticleVelocity = 50.0f;
  emitter->particleInitialSize = { 2.0f, 2.0f };
  emitter->particleSecondsToLive = 1.0f;
}

void DamageSystem::PlayAsteroidDeathSound(const IdentityComponent* asteroidIdentity)
{
  switch (asteroidIdentity->identity)
  {
  case Identity::BigAsteroid:
    Sound::PlaySound(pContext, L"BigAsteroidDeathSound");
    break;
  case Identity::MediumAsteroid:
    Sound::PlaySound(pContext, L"MediumAsteroidDeathSound");
    break;
  case Identity::SmallAsteroid:
    Sound::PlaySound(pContext, L"SmallAsteroidDeathSound");
    break;
  }
}