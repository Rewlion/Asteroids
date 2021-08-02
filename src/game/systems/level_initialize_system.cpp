#include "level_initialize_system.h"

#include <engine/renderer/field_component.h>
#include <engine/ecs/Context.h>
#include <engine/sound/utils.h>
#include <game/components/spawn_request_component.h>
#include <game/game_objects_shapes.h>
#include <game/components/game_statistics_component.h>
#include <game/components/gui_update_event_components.h>

LevelInitializeSystem::LevelInitializeSystem(Context* ecsContext)
  : InitializationSystem(ecsContext)
{

}

void LevelInitializeSystem::Initialize()
{
  EntityManager* em = pContext->GetEntityManager();
  Entity* e = em->NewEntity();
  FieldComponent* field = e->AddComponent<FieldComponent>("Field Component");
  field->gameFieldSize = { 800, 600 };
  GameStatisticsComponent* stats = e->AddComponent<GameStatisticsComponent>("Game Statistics Component");
  stats->playerScore = 0;
  stats->playerLifes = 5;
  stats->gameStatus = GameStatus::Play;

  SpawnRequestComponent* requestToSpawnPlayer = em->NewEntity()->AddComponent<SpawnRequestComponent>("Request to Spawn player");
  requestToSpawnPlayer->objectType = Identity::PlayerShip;
  requestToSpawnPlayer->team = Team::Team0;
  requestToSpawnPlayer->position = { 400, 300 };
  requestToSpawnPlayer->rotation = 0.0f;

  GuiUpdatePlayerLifesComponent* updatePlayerLifesOnGuiRequest = em->NewEntity()->AddComponent<GuiUpdatePlayerLifesComponent>("update player lifes");
  updatePlayerLifesOnGuiRequest->newPlayerLifes = stats->playerLifes;

  GuiUpdateScoreComponent* updateScoreOnGuiRequest = em->NewEntity()->AddComponent<GuiUpdateScoreComponent>("update player lifes");
  updateScoreOnGuiRequest->newScore = 0;

  Sound::LoadSound(pContext, L"resources/sound/fire.wav", L"FireSound");
  Sound::LoadSound(pContext, L"resources/sound/thrust.wav", L"ThrustSound");
  Sound::LoadSound(pContext, L"resources/sound/bangSmall.wav", L"SmallAsteroidDeathSound");
  Sound::LoadSound(pContext, L"resources/sound/bangMedium.wav", L"MediumAsteroidDeathSound");
  Sound::LoadSound(pContext, L"resources/sound/bangLarge.wav", L"BigAsteroidDeathSound");
}

