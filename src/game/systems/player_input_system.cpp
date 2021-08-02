#include "player_input_system.h"
#include <engine/ecs/Context.h>

#include <engine/scene_component.h>
#include <engine/collision/circle_collision_component.h>
#include <engine/renderer/particles/direct_emitter_component.h>
#include <engine/sound/utils.h>
#include <game/components/player_component.h>
#include <game/components/movement_component.h>
#include <game/components/spawn_request_component.h>

namespace Input
{
  PlayerInputSystem::PlayerInputSystem(Context* ecsContext)
    : BaseInputSystem(ecsContext)
    , m_PlayerGroup(ecsContext->GetGroup<MovementComponent, PlayerComponent, CircleCollisionComponent>())
    , m_ShipRotationFlag(0.0f)
    , m_ShipVelocityFlag(0.0f)
  {
  }

  void PlayerInputSystem::OnInput(Key key, KeyAction action)
  {
    if (key == Key::D)
    {
      if (action == KeyAction::Press)
        m_ShipRotationFlag += 1.0f;
      else if (action == KeyAction::Release)
        m_ShipRotationFlag -= 1.0f;
    }
    else if (key == Key::A)
    {
      if (action == KeyAction::Press)
        m_ShipRotationFlag -= 1.0f;
      else if (action == KeyAction::Release)
        m_ShipRotationFlag += 1.0f;
    }
    else if (key == Key::W)
    {
      if (action == KeyAction::Press)
        m_ShipVelocityFlag += 1.0f;
      else if (action == KeyAction::Release)
        m_ShipVelocityFlag -= 1.0f;
    }
    else if (key == Key::S)
    {
      if (action == KeyAction::Press)
        m_ShipVelocityFlag -= 1.0f;
      else if (action == KeyAction::Release)
        m_ShipVelocityFlag += 1.0f;
    }
    else if (key == Key::SPACE && action == KeyAction::Press)
    {
      Entity* player = m_PlayerGroup->GetFirstNotNullEntity();
      if (!player)
        return;

      SpawnRequestComponent* request = pContext->GetEntityManager()->NewEntity()->AddComponent<SpawnRequestComponent>("Request to spawn a bullet");
      SceneComponent* root = player->GetFirstComponent<SceneComponent>();
      CircleCollisionComponent* collisionShape = player->GetFirstComponent<CircleCollisionComponent>();

      request->objectType = Identity::Bullet;
      request->team = Team::Team0;
      request->position = root->GetLocalLocation() + root->GetForward() * (collisionShape->radius + 3.0f);
      request->rotation = root->GetLocalRotation();

      Sound::PlaySound(pContext, L"FireSound");
    }
  }

  void PlayerInputSystem::Update(const double dt)
  {
    Entity* player = m_PlayerGroup->GetFirstNotNullEntity();
    if (!player)
      return;

    MovementComponent* shipEngine = player->GetFirstComponent<MovementComponent>();
    shipEngine->rotationVelocity = m_ShipRotationFlag * shipEngine->rorationMaxVelocity;
    shipEngine->acceleration = m_ShipVelocityFlag * shipEngine->maxAcceleration;

    DirectParticleEmitterComponent* engineSmoke = player->GetFirstComponent<DirectParticleEmitterComponent>();
    engineSmoke->isEnabled = !!m_ShipVelocityFlag;
  }
}
