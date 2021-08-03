#include "asteroids_spawn_system.h"

#include <engine/ecs/Context.h>
#include <engine/collision/entity_collides_component.h>
#include <engine/utils/time/utils.h>
#include <engine/renderer/field_component.h>
#include <game/components/identity_component.h>
#include <game/components/spawn_request_component.h>
#include <math/math.hpp>

#include <random>

namespace
{
  Math::fVec2 GetRandomPositionOnQuadBorder(Math::fVec2 gameFieldSize)
  {
    Math::fVec2 position;

    const unsigned char k = std::rand() % 4;
    const float randX = static_cast<float>(std::rand() % static_cast<int>(gameFieldSize.x));
    const float randY = static_cast<float>(std::rand() % static_cast<int>(gameFieldSize.y));

    switch (k)
    {
    case 0:
      position = Math::fVec2{ randX , gameFieldSize.y };
      break;
    case 1:
      position = Math::fVec2{ randX , 0 };
      break;
    case 2:
      position = Math::fVec2{ gameFieldSize.x, randY };
      break;
    case 3:
      position = Math::fVec2{ 0, randY };
      break;
    }

    return position;
  }
}

AsteroidsSpawnSystem::AsteroidsSpawnSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_IdentityGroup(ecsContext->GetGroup<IdentityComponent>())
  , m_GameFieldGroup(ecsContext->GetGroup<FieldComponent>())
{
}

void AsteroidsSpawnSystem::AsteroidsSpawnSystem::Update(const float dt)
{
  unsigned int bigAsteroidsCount = 0;
  for(Entity* e: m_IdentityGroup->GetEntities())
  if (e)
  {
    bigAsteroidsCount += e->GetFirstComponent<IdentityComponent>()->identity == Identity::BigAsteroid;
  }

  if (bigAsteroidsCount == 0 && !m_TimerToSpawnAsteroidsAlreadySetup)
  {
    FieldComponent* gameField = m_GameFieldGroup->GetFirstNotNullEntity()->GetFirstComponent<FieldComponent>();

    Time::SetupDoOnceTimer(pContext, 1.0f, "Spawn Asteroids Clock", [gameField, this](Context* ecsContext) {
      const int nAsteroidsToSpawn = 6;

      for (int i = 0; i < nAsteroidsToSpawn; ++i)
      {
        const float rotation = static_cast<float>(std::rand() % 360);
        const Math::fVec2 position = GetRandomPositionOnQuadBorder(gameField->gameFieldSize);

        SpawnRequestComponent* request = pContext->GetEntityManager()->NewEntity()->AddComponent<SpawnRequestComponent>("Spawn Asteroid Request");
        request->objectType = Identity::BigAsteroid;
        request->position = position;
        request->rotation = rotation;
        request->team = Team::Team1;
      }
      });

    m_TimerToSpawnAsteroidsAlreadySetup = true;
  }
  else if (bigAsteroidsCount > 0 && m_TimerToSpawnAsteroidsAlreadySetup)
    m_TimerToSpawnAsteroidsAlreadySetup = false;
}

