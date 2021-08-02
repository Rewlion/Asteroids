#include "field_border_teleport_system.h"

#include <engine/ecs/Context.h>
#include <engine/ecs/Group.h>
#include <engine/scene_component.h>
#include <engine/renderer/field_component.h>
#include <game/components/identity_component.h>

FieldBorderTeleportSystem::FieldBorderTeleportSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_ObjectsSceneComponentGroup(ecsContext->GetGroup<SceneComponent, IdentityComponent>())
  , m_GameFieldGroup(ecsContext->GetGroup<FieldComponent>())
{
}

void FieldBorderTeleportSystem::Update(const double dt)
{
  FieldComponent* gameField = m_GameFieldGroup->GetFirstNotNullEntity()->GetFirstComponent<FieldComponent>();

  for(Entity* e: m_ObjectsSceneComponentGroup->GetEntities())
  if (e && e->IsActivated())
  {
    const float fieldEps = 10.0f;
    SceneComponent* root = e->GetFirstComponent<SceneComponent>();

    Math::fVec2 pos = root->GetLocalLocation();

    if (pos.y > gameField->gameFieldSize.y + fieldEps)
      pos.y = 0.0f - fieldEps - .2f;
    else if (pos.y < 0.0f - fieldEps)
      pos.y = gameField->gameFieldSize.y + fieldEps - .2f;

    if (pos.x > gameField->gameFieldSize.x + fieldEps)
      pos.x = 0.0f - fieldEps - .2f;
    else if (pos.x < 0.0f - fieldEps)
      pos.x = gameField->gameFieldSize.x + fieldEps - .2f;

    root->SetLocalLocation(pos);
  }
}
