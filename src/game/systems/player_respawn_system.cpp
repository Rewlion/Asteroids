#include "player_respawn_system.h"

#include <engine/ecs/Context.h>
#include <engine/collision/entity_collides_component.h>
#include <engine/utils/time/utils.h>
#include <engine/renderer/field_component.h>
#include <game/components/player_component.h>
#include <game/components/spawn_request_component.h>
#include <game/components/game_statistics_component.h>
#include <math/math.hpp>

#include <random>

PlayerRespawnSystem::PlayerRespawnSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_PlayerGroup(ecsContext->GetGroup<PlayerComponent>())
  , m_GameStatisticsGroup(ecsContext->GetGroup<GameStatisticsComponent>())
  , m_GameFieldGroup(ecsContext->GetGroup<FieldComponent>())
{
}

void PlayerRespawnSystem::Update(const float dt)
{
  const bool isPlayerAlive = m_PlayerGroup->GetFirstNotNullEntity() != nullptr;
  const auto* gameStatistics = m_GameStatisticsGroup->GetFirstNotNullEntity()->GetFirstComponent<GameStatisticsComponent>();
  const auto* gameField = m_GameFieldGroup->GetFirstNotNullEntity()->GetFirstComponent<FieldComponent>();

  if (!isPlayerAlive && !m_TimerToSpawnPlayerAlreadySetup && gameStatistics->playerLifes > 0)
  {
    Time::SetupDoOnceTimer(pContext, 1.0f, "Spawn Player Clock", [this, gameStatistics, gameField](Context* ecsContext) {
        SpawnRequestComponent* request = pContext->GetEntityManager()->NewEntity()->AddComponent<SpawnRequestComponent>("Spawn Player Request");
        request->objectType = Identity::PlayerShip;
        request->position = { gameField->gameFieldSize.x / 2.0f, gameField->gameFieldSize.y / 2.0f };
        request->rotation = 180.0f;
        request->team = Team::Team0;
    });

    m_TimerToSpawnPlayerAlreadySetup = true;
  }
  else if (isPlayerAlive && m_TimerToSpawnPlayerAlreadySetup)
    m_TimerToSpawnPlayerAlreadySetup = false;
}

