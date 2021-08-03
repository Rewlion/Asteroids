#include "base_input_system.h"

#include <engine/ecs/Context.h>
#include <engine/engine_events.h>

namespace Input
{
  BaseInputSystem::BaseInputSystem(Context* pcontext)
    : LogicSystem(pcontext)
    , m_InputObserver(pcontext->GetEventManager(), EventObservingPriority::Priority_Critical)
  {
    m_InputObserver.OnEvent<InputEvent>([&](BaseEvent* e) {
      InputEvent* ie = reinterpret_cast<InputEvent*>(e);
      OnInput(ie->key, ie->action);
    });
  }
}
