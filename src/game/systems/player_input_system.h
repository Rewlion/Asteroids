#pragma once

#include <engine/window/base_input_system.h>
#include <engine/ecs/Group.h>
#include <math/math.hpp>

namespace Input
{
  class PlayerInputSystem : public BaseInputSystem
  {
  public:
    PlayerInputSystem(Context* ecsContext);

    virtual void OnInput(Key key, KeyAction action) override;
    virtual void Update(const float dt) override;

  private:
    Group* m_PlayerGroup;
    float m_ShipRotationFlag;
    float m_ShipVelocityFlag;
  };
}
