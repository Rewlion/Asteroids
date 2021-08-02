#include "timer_system.h"
#include "timer_components.h"

#include <engine/ecs/Context.h>
#include <engine/ecs/Group.h>

TimerSystem::TimerSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_DoOnceTimerGroup(ecsContext->GetGroup<DoOnceTimerComponent>())
{
}

void TimerSystem::Update(const double dt)
{
  for(Entity* e: m_DoOnceTimerGroup->GetEntities())
  if (e)
  {
    auto* timer = e->GetFirstComponent<DoOnceTimerComponent>();
    timer->tickedTime += dt;

    if (timer->tickedTime > timer->secondsBeforeFire)
    {
      timer->callback(pContext);
      pContext->GetEntityManager()->ReleaseEntity(e);
    }
  }
}
