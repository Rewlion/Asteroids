#include "asteroids_spawn_system.h"

#include <engine/ecs/Context.h>
#include <engine/collision/entity_collides_component.h>
#include <engine/utils/time/utils.h>
#include <engine/renderer/field_component.h>
#include <game/components/identity_component.h>
#include <game/components/spawn_request_component.h>
#include <math/math.hpp>

#include <random>

AsteroidsSpawnSystem::AsteroidsSpawnSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_IdentityGroup(ecsContext->GetGroup<IdentityComponent>())
  , m_GameFieldGroup(ecsContext->GetGroup<FieldComponent>())
{
}

void AsteroidsSpawnSystem::AsteroidsSpawnSystem::Update(const double dt)
{
  unsigned int asteroidsCount = 0;
  for(Entity* e: m_IdentityGroup->GetEntities())
  if (e)
  {
    asteroidsCount += e->GetFirstComponent<IdentityComponent>()->identity == Identity::BigAsteroid;
  }

  if (asteroidsCount == 0 && !m_TimerToSpawnAsteroidsAlreadySetup)
  {
    FieldComponent* gameField = m_GameFieldGroup->GetFirstNotNullEntity()->GetFirstComponent<FieldComponent>();

    Time::SetupDoOnceTimer(pContext, 1.0f, "Spawn Asteroids Clock", [gameField, this](Context* ecsContext) {
      const int nAsteroidsToSpawn = 6;

      for (int i = 0; i < nAsteroidsToSpawn; ++i)
      {
        const float rotation = std::rand() % 360;

        Math::fVec2 position;
        const unsigned char k = std::rand() % 4;
        switch (k)
        {
        case 0:
          position = Math::fVec2{ float(std::rand() % static_cast<int>(gameField->gameFieldSize.x)) , gameField->gameFieldSize.y };
          break;
        case 1:
          position = Math::fVec2{ float(std::rand() % static_cast<int>(gameField->gameFieldSize.x)) , 0 };
          break;
        case 2:
          position = Math::fVec2{ gameField->gameFieldSize.x, float(std::rand() % static_cast<int>(gameField->gameFieldSize.y)) };
          break;
        case 3:
          position = Math::fVec2{ 0, float(std::rand() % static_cast<int>(gameField->gameFieldSize.y)) };
          break;
        }

        SpawnRequestComponent* request = pContext->GetEntityManager()->NewEntity()->AddComponent<SpawnRequestComponent>("Spawn Asteroid Request");
        request->objectType = Identity::BigAsteroid;
        request->position = position;
        request->rotation = rotation;
        request->team = Team::Team1;
      }
      });

    m_TimerToSpawnAsteroidsAlreadySetup = true;
  }
  else if (asteroidsCount > 0 && m_TimerToSpawnAsteroidsAlreadySetup)
    m_TimerToSpawnAsteroidsAlreadySetup = false;
}

