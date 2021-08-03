#include "movement_system.h"

#include <game/components/movement_component.h>
#include <engine/ecs/Context.h>
#include <engine/scene_component.h>

MovementSystem::MovementSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_MovementGroup(ecsContext->GetGroup<MovementComponent, SceneComponent>())
  , m_ConstantMovementGroup(ecsContext->GetGroup<ConstantMovementComponent, SceneComponent>())
{
}

void MovementSystem::Update(const float dt)
{
  for(Entity* e: m_MovementGroup->GetEntities())
  if (e && e->IsActivated())
  {
    auto* movement = e->GetFirstComponent<MovementComponent>();
    auto* root = e->GetFirstComponent<SceneComponent>();

    const float r = root->GetLocalRotation() + movement->rotationVelocity * dt;
    root->SetLocalRotation(r);

    Math::fVec2 dv = { 0.0f, 0.0f };
    if (movement->acceleration != 0)
      dv = root->GetForward() * (movement->acceleration * dt);
    else if (movement->velocity.Length2() > 0.1)
      dv = -1.0f * movement->velocity.Normalize() * (movement->maxAcceleration * 1.5f * dt);
    else
      dv = -1.0f * movement->velocity;

    movement->velocity += dv;

    const Math::fVec2 newPos = root->GetWorldLocation() + movement->velocity * dt;
    root->SetLocalLocation(newPos);
  }

  for(Entity* e: m_ConstantMovementGroup->GetEntities())
  if (e && e->IsActivated())
  {
    auto* movement = e->GetFirstComponent<ConstantMovementComponent>();
    auto* root = e->GetFirstComponent<SceneComponent>();

    const Math::fVec2 newPos = root->GetWorldLocation() + movement->velocity * dt;
    root->SetLocalLocation(newPos);

    root->SetLocalLocation(newPos);
  }
}
