#include "death_clock_system.h"

#include <engine/ecs/Context.h>
#include <game/components/death_clock_component.h>

DeathClockSystem::DeathClockSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_DeathClockGroup(ecsContext->GetGroup<DeathClockComponent>())
{
}

void DeathClockSystem::Update(const double dt)
{
  for(Entity* e: m_DeathClockGroup->GetEntities())
  if (e)
  {
    DeathClockComponent* clock = e->GetFirstComponent<DeathClockComponent>();
    clock->tickedTime += dt;

    if (clock->tickedTime > clock->timeToLive)
      pContext->GetEntityManager()->ReleaseEntity(e);
  }
}
