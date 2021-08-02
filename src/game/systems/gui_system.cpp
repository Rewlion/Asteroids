#include "gui_system.h"

#include <engine/ecs/Context.h>
#include <engine/renderer/field_component.h>
#include <engine/renderer/text_widget_component.h>
#include <game/components/game_statistics_component.h>
#include <game/components/gui_update_event_components.h>
#include <game/game_objects_shapes.h>
#include <game/components/game_statistics_component.h>

namespace
{
  Entity* AddTextWidget(EntityManager* em, const char* name, const unsigned int size, const wchar_t* text, const Math::fVec2 position, const Color color)
  {
    Entity* e = em->NewEntity();
    auto* widget = e->AddComponent<TextWidgetComponent>(name);
    widget->size = size;
    widget->text = text;
    widget->position = position;
    widget->color = color;

    return e;
  }

  template<class T>
  T* GetFirstComponent(Group* group)
  {
    Entity* e = group->GetFirstNotNullEntity();
    if (e)
      return e->GetFirstComponent<T>();

    return nullptr;
  }

  void ClearGroup(Context* ecsContext, Group* group)
  {
    for (Entity* e : group->GetEntities())
    if (e)
    ecsContext->GetEntityManager()->ReleaseEntity(e);
  }
}

GuiSystem::GuiSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_GameStatisticsGroup(ecsContext->GetGroup<GameStatisticsComponent>())
  , m_GameFieldGroup(ecsContext->GetGroup<FieldComponent>())
  , m_UpdatePlayerLifesEventsGroup(ecsContext->GetGroup<GuiUpdatePlayerLifesComponent>())
  , m_UpdateScoreEventsGroup(ecsContext->GetGroup<GuiUpdateScoreComponent>())
  , m_ScoreWidget(nullptr)
{

}

void GuiSystem::Update(const double dt)
{
  const auto* gameField = m_GameFieldGroup->GetFirstNotNullEntity()->GetFirstComponent<FieldComponent>();

  if (m_ScoreWidget == nullptr)
  {
    Math::fVec2 textPos = { gameField->gameFieldSize.x * 0.05f , gameField->gameFieldSize.y * 0.05f - gameField->gameFieldSize.x / 120.0f };
    m_ScoreWidget = AddTextWidget(pContext->GetEntityManager(), "Score Widget", 16, L"", textPos, Color::Chocolate);

    textPos = { gameField->gameFieldSize.x * 0.4f, gameField->gameFieldSize.y * 0.9f };
    AddTextWidget(pContext->GetEntityManager(), "kiki Widget", 10, L"% DEFINITELY NOT ATARI", textPos, Color::Chocolate);
  }

  auto* updateScoreEvent = GetFirstComponent<GuiUpdateScoreComponent>(m_UpdateScoreEventsGroup);
  if (updateScoreEvent)
  {
    auto* scoreWidget = m_ScoreWidget->GetFirstComponent<TextWidgetComponent>();
    scoreWidget->text = std::wstring(L"SCORE: ") + std::to_wstring(updateScoreEvent->newScore);
    ClearGroup(pContext, m_UpdateScoreEventsGroup);
  }

  auto* updatePlayerLifesEvent = GetFirstComponent<GuiUpdatePlayerLifesComponent>(m_UpdatePlayerLifesEventsGroup);
  if (updatePlayerLifesEvent)
  {
    ReconstructPlayerLifesWidget(updatePlayerLifesEvent->newPlayerLifes);
    ClearGroup(pContext, m_UpdatePlayerLifesEventsGroup);
  }

  const auto* gameStats = GetFirstComponent<GameStatisticsComponent>(m_GameStatisticsGroup);
  if (gameStats->gameStatus == GameStatus::Lose && !m_LoseWidget)
  {
    Math::fVec2 textPos = { gameField->gameFieldSize.x * 0.4f, gameField->gameFieldSize.y * 0.5f };
    m_LoseWidget= AddTextWidget(pContext->GetEntityManager(), "Lose Widget", 30, L"LOSE", textPos, Color::WhiteSmoke);
  }
}

void GuiSystem::ReconstructPlayerLifesWidget(const unsigned int newLifesCount)
{
  for (auto* e : m_PlayerLifesWidget)
    pContext->GetEntityManager()->ReleaseEntity(e);
  m_PlayerLifesWidget.clear();

  const auto* gameField = m_GameFieldGroup->GetFirstNotNullEntity()->GetFirstComponent<FieldComponent>();
  const float elementSize = gameField->gameFieldSize.x / 60.0f;
  const float distanceBetweenElements = 5.0f;
  const Math::fVec2 firstElementPosition = {gameField->gameFieldSize.x * 0.8f, gameField->gameFieldSize.y * 0.05f };

  const std::vector<StripLinesComponent::Point> model = Game::Utils::DefaultGameObjectsSetttings.at(Identity::PlayerShip).model;

  for (int i = 0; i < newLifesCount; ++i)
  {
    Entity* e = pContext->GetEntityManager()->NewEntity();
    StripLinesComponent* elementModel = e->AddComponent<StripLinesComponent>("player life widget element");
    elementModel->Detach();

    Math::fVec2 elementPosition{
      firstElementPosition.x + i * (elementSize + distanceBetweenElements),
      firstElementPosition.y
    };

    elementModel->SetLocalLocation(elementPosition);
    elementModel->SetLocalRotation(180.0f);
    elementModel->SetLocalSize({ elementSize, elementSize });
    elementModel->SetPoints(model);
    elementModel->SetColor(Color::Chocolate);

    m_PlayerLifesWidget.push_back(e);
  }
}
