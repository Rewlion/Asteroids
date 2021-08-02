#include "collision_system.h"
#include "circle_collision_component.h"

#include <engine/ecs/BaseSystems.h>
#include <engine/collision/entity_collides_component.h>
#include <engine/scene_component.h>
#include <engine/ecs/Context.h>
#include <engine/ecs/Group.h>

namespace
{
  class CollisionEventsCleaner : public CleanupSystem
  {
  public:
    CollisionEventsCleaner(Context* ecsContext)
      : CleanupSystem(ecsContext)
      , m_CollisionEventsGroup(ecsContext->GetGroup<EntityCollidesComponent>())
    {
    }

    virtual void Clean() override
    {
      for (Entity* e : m_CollisionEventsGroup->GetEntities())
      if (e)
      {
        auto comps = e->GetComponents<EntityCollidesComponent>();
        for (EntityCollidesComponent* comp : comps)
          e->RemoveComponent(comp->Name);
      }
    }

  private:
    Group* m_CollisionEventsGroup;
  };

  void AddNewColliderEntity(Entity* to, Entity* collidedWithEntity)
  {
    auto* collisionEvent = to->GetFirstComponent<EntityCollidesComponent>();
    if (!collisionEvent)
    {
      collisionEvent = to->AddComponent<EntityCollidesComponent>("collision event");
      collisionEvent->entitiesCollidedWith.clear();
      collisionEvent->entitiesCollidedWith.push_back(collidedWithEntity);
    }
    else
    {
      auto& collisions = collisionEvent->entitiesCollidedWith;
      if (collisions.end() == std::find(collisions.begin(), collisions.end(), collidedWithEntity))
        collisions.push_back(collidedWithEntity);
    }
  }
}

CollisionSystem::CollisionSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_CircleComponentGroup(ecsContext->GetGroup<CircleCollisionComponent, SceneComponent>())
{
  ecsContext->AddCleanupSystems({ new CollisionEventsCleaner(ecsContext) });
}

void CollisionSystem::Update(const double dt)
{
  for(Entity* e1: m_CircleComponentGroup->GetEntities())
  if (e1)
  for (Entity* e2 : m_CircleComponentGroup->GetEntities())
  if (e2 && e2 != e1)
  {
    const Math::fVec2 pos1 = e1->GetFirstComponent<SceneComponent>()->GetWorldLocation();
    const float r1 = e1->GetFirstComponent<CircleCollisionComponent>()->radius;
    const Math::fVec2 pos2 = e2->GetFirstComponent<SceneComponent>()->GetWorldLocation();
    const float r2 = e2->GetFirstComponent<CircleCollisionComponent>()->radius;


    const float dp = (pos1 - pos2).Length2();
    const float r1r2 = (r1+r2) * (r1+r2);

    if (dp < r1r2)
    {
      AddNewColliderEntity(e1, e2);
      AddNewColliderEntity(e2, e1);
    }
  }
}
