#include "utils.h"
#include "timer_components.h"

#include <engine/ecs/Context.h>

namespace Time
{
  Entity* SetupDoOnceTimer(Context* ecsContext, const float secondsBeforeFire, const char* timerName, TimerCallback callback)
  {
    Entity* e = ecsContext->GetEntityManager()->NewEntity();
    DoOnceTimerComponent* timer = e->AddComponent<DoOnceTimerComponent>(timerName);
    timer->tickedTime = 0.0f;
    timer->secondsBeforeFire = secondsBeforeFire;
    timer->callback = callback;

    return e;
  }
}
