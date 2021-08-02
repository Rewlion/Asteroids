#include "game_objects_spawn_system.h"

#include <engine/collision/circle_collision_component.h>
#include <engine/ecs/Context.h>
#include <engine/ecs/Group.h>
#include <engine/scene_component.h>
#include <engine/renderer/strip_lines_component.h>
#include <engine/renderer/particles/direct_emitter_component.h>
#include <game/components/spawn_request_component.h>
#include <game/components/player_component.h>
#include <game/components/movement_component.h>
#include <game/components/identity_component.h>
#include <game/components/death_clock_component.h>
#include <game/game_objects_shapes.h>

namespace
{
  void SpawnAsteroid(EntityManager* em, const SpawnRequestComponent* request, const float velocity)
  {
    Entity* asteroid = em->NewEntity();

    SceneComponent* root = asteroid->AddComponent<SceneComponent>("Root Component");
    root->SetLocalLocation(request->position);
    root->SetLocalRotation(request->rotation);
    root->SetLocalSize({ 1.0f, 1.0f });

    const Game::Utils::GameObjectSettings& defaultSettings = Game::Utils::DefaultGameObjectsSetttings.at(request->objectType);

    StripLinesComponent* shipModel = asteroid->AddComponent<StripLinesComponent>("Ship Model");
    shipModel->AttachTo(root);
    shipModel->SetPoints(defaultSettings.model);
    shipModel->SetLocalSize(defaultSettings.size);
    shipModel->SetColor(Color::Cyan);

    ConstantMovementComponent* movement = asteroid->AddComponent<ConstantMovementComponent>("Bullet movement");
    movement->velocity = Math::RotateVector({ 0.0f, 1.0f }, request->rotation) * velocity;

    CircleCollisionComponent* collisionShape = asteroid->AddComponent<CircleCollisionComponent>("Circle Collision");
    collisionShape->radius = defaultSettings.circleCollisionRadius;

    IdentityComponent* identity = asteroid->AddComponent<IdentityComponent>("Identity");
    identity->identity = request->objectType;
    identity->team = request->team;
  }

  void SpawnBullet(EntityManager* em, const SpawnRequestComponent* request, const float velocity)
  {
    Entity* bullet = em->NewEntity();

    SceneComponent* root = bullet->AddComponent<SceneComponent>("Root Component");
    root->Detach();
    root->SetLocalLocation(request->position);
    root->SetLocalRotation(request->rotation);
    root->SetLocalSize({ 1.0f, 1.0f });

    const Game::Utils::GameObjectSettings& defaultSettings = Game::Utils::DefaultGameObjectsSetttings.at(request->objectType);

    StripLinesComponent* bulletModel = bullet->AddComponent<StripLinesComponent>("Bullet Model");
    bulletModel->AttachTo(root);
    bulletModel->SetPoints(defaultSettings.model);
    bulletModel->SetLocalSize(defaultSettings.size);
    bulletModel->SetColor(Color::Bisque);
    bulletModel->SetLocalRotation(0.0f);

    ConstantMovementComponent* movement = bullet->AddComponent<ConstantMovementComponent>("Bullet movement");
    movement->velocity = Math::RotateVector({ 0.0f, 1.0f }, request->rotation) * velocity;

    CircleCollisionComponent* collisionShape = bullet->AddComponent<CircleCollisionComponent>("Circle Collision");
    collisionShape->radius = defaultSettings.circleCollisionRadius;

    IdentityComponent* identity = bullet->AddComponent<IdentityComponent>("Identity");
    identity->identity = request->objectType;
    identity->team = request->team;

    DeathClockComponent* deathClock = bullet->AddComponent<DeathClockComponent>("Clock to die");
    deathClock->timeToLive = 1.0f;
    deathClock->tickedTime = 0.0f;
  }

  void SpawnPlayerShip(EntityManager* em, const SpawnRequestComponent* request)
  {
    Entity* player = em->NewEntity();
    player->AddComponent<PlayerComponent>("Player Component");

    SceneComponent* root = player->AddComponent<SceneComponent>("Root Component");
    root->Detach();
    root->SetLocalSize({ 1.0f, 1.0f });
    root->SetLocalLocation(request->position);
    root->SetLocalRotation(request->rotation);

    const Game::Utils::GameObjectSettings& defaultSettings = Game::Utils::DefaultGameObjectsSetttings.at(request->objectType);

    StripLinesComponent* shipModel = player->AddComponent<StripLinesComponent>("Ship Model");
    shipModel->AttachTo(root);
    shipModel->SetPoints(defaultSettings.model);
    shipModel->SetLocalSize(defaultSettings.size);
    shipModel->SetLocalLocation({ 0.0f, 0.0f });
    shipModel->SetLocalRotation(0.0f);
    shipModel->SetColor(Color::Bisque);

    MovementComponent* shipEngine = player->AddComponent<MovementComponent>("Engine");
    shipEngine->maxAcceleration = 300.0f;
    shipEngine->rorationMaxVelocity = 200.0f;
    shipEngine->rotationVelocity = 0.0f;
    shipEngine->velocity = { 0.0f, 0.0f };
    shipEngine->acceleration = 0.0f;

    CircleCollisionComponent* collisionShape = player->AddComponent<CircleCollisionComponent>("Circle Collision");
    collisionShape->radius = defaultSettings.circleCollisionRadius;

    IdentityComponent* identity = player->AddComponent<IdentityComponent>("Identity");
    identity->identity = request->objectType;
    identity->team = request->team;

    DirectParticleEmitterComponent* emitter = player->AddComponent<DirectParticleEmitterComponent>("Engine smoke emitter");
    emitter->isEnabled = true;
    emitter->secondsSinceLastEmmit = 0.0f;
    emitter->secondsToEmmit = 0.05f;
    emitter->particleModel = { {-0.5f, -0.5f}, {0.0f, 0.5f}, {0.5f,-0.5f}, {-0.5f, -0.5f} };
    emitter->particleInitialSize = defaultSettings.size * 0.4f;
    emitter->particleSecondsToLive = 1.0f;
    emitter->particleSizeDecreaseSpeed = 0.1f;
    emitter->particleVelocity = 10.0f;
    emitter->isInitialRotationRandomized = true;
    emitter->AttachTo(root);
    emitter->SetLocalLocation({ 0.0f, -defaultSettings.size.y * 1.2f });
    emitter->SetLocalRotation(0.0f);
    emitter->SetLocalSize({ 1.0f, 1.0f });
  }
}

GameObjectsSpawnSystem::GameObjectsSpawnSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_SpawnRequestGroup(ecsContext->GetGroup<SpawnRequestComponent>())
{
}

void GameObjectsSpawnSystem::Update(const double dt)
{
  for (Entity* e: m_SpawnRequestGroup->GetEntities())
  if (e && e->IsActivated())
  {
    const auto requests = e->GetComponents<SpawnRequestComponent>();
    for (const auto req : requests)
    {
      Spawn(req);
    }

    pContext->GetEntityManager()->ReleaseEntity(e);
  }
}

void GameObjectsSpawnSystem::Spawn(const SpawnRequestComponent* request)
{
  EntityManager* em = pContext->GetEntityManager();

  switch (request->objectType)
  {
    case Identity::PlayerShip:
    {
      SpawnPlayerShip(em, request);
      break;
    }

    case Identity::Bullet:
    {
      SpawnBullet(em, request, 600.0f);
      break;
    }

    case Identity::BigAsteroid:
    {
      SpawnAsteroid(em, request, 80.0f);
      break;
    }

    case Identity::MediumAsteroid:
    {
      SpawnAsteroid(em, request, 120.0f);
      break;
    }

    case Identity::SmallAsteroid:
    {
      SpawnAsteroid(em, request, 150.0f);
      break;
    }

    default:
      assert(!"unknwon object");
  }
}
