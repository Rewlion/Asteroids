#pragma once

#include <engine/ecs/BaseSystems.h>
#include <engine/ecs/Events/EventManagement.h>
#include <engine/window/input_utility.h>

namespace Input
{
  class BaseInputSystem : public LogicSystem
  {
  public:
    BaseInputSystem(Context* pcontext);

    virtual void OnInput(Key key, KeyAction action) {};

  private:
    EventObserver m_InputObserver;
  };
}
