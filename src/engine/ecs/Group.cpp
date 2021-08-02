#include "Group.h"
#include "Context.h"
#include "ECSEvents.h"
#include "Entity.h"

Group::Group(Context* context, const std::vector<ComponentID>& requirableComponentIDs)
    : CurrentContext(context)
    , RequirableComponentIDs(requirableComponentIDs)
    , Observer(CurrentContext->GetEventManager(), Priority_Critical)
{
    Observer.OnEvent<EntityAddedEvent>([this](BaseEvent* event)
    {
        OnEntityAdded(event);
    });

    Observer.OnEvent<EntityReleasedEvent>([this](BaseEvent* event)
    {
        OnEntityReleased(event);
    });

    Observer.OnEvent<ComponentAddedEvent>([this](BaseEvent* event)
    {
        OnComponentAdded(event);
    });

    Observer.OnEvent<ComponentRemovedEvent>([this](BaseEvent* event)
    {
        OnComponentRemoved(event);
    });

    const auto entities = context->GetEntityManager()->GetEntities();
    for (const auto& entity : entities)
    {
        if (entity->HasComponents(RequirableComponentIDs))
            Entities.push_back(entity);
    }
}

void Group::OnEntityAdded(BaseEvent* event)
{
    EntityAddedEvent* castedEvent = reinterpret_cast<EntityAddedEvent*>(event);
    assert(castedEvent != nullptr);
    Entity* addedEntity = castedEvent->AddedEntity;

    if (addedEntity->HasComponents(RequirableComponentIDs))
    {
        if (ReusableEntityIds.empty() == false)
        {
            const PositionInCache position = ReusableEntityIds.top();
            assert((Entities[position] == nullptr) && "Group cached ids has an activated entity.");
            Entities[position] = addedEntity;

            return;
        }
        else
        {
            Entities.push_back(addedEntity);
        }
    }
}

void Group::OnEntityReleased(BaseEvent* event)
{
    EntityReleasedEvent* castedEvent = reinterpret_cast<EntityReleasedEvent*>(event);
    assert(castedEvent != nullptr);
    const Entity* removedEntity = castedEvent->RemovedEntity;

    auto [positionInCache, isFound] = FindPositionWithID(removedEntity->GetId());
    if (isFound)
    {
        Entities[positionInCache] = nullptr;
        ReusableEntityIds.push(positionInCache);
    }
}

void Group::OnComponentAdded(BaseEvent* event)
{
    ComponentAddedEvent* castedEvent = reinterpret_cast<ComponentAddedEvent*>(event);
    Entity* owner = castedEvent->Owner; // ownerid -> owner(entity)

    auto[positionInCache, isFound] = FindPositionWithID(owner->GetId());
    if (isFound == false)
    {
        if (owner->HasComponents(RequirableComponentIDs))
        {
            if (ReusableEntityIds.empty() == false)
            {
                const PositionInCache position = ReusableEntityIds.top();
                assert((Entities[position] == nullptr) && "Removed entity in group is not nullptr.");
                Entities[position] = owner;
                ReusableEntityIds.pop();

                return;
            }
            else
            {
                Entities.push_back(owner);
            }
        }
    }
}

void Group::OnComponentRemoved(BaseEvent* event)
{
    ComponentRemovedEvent* castedEvent = reinterpret_cast<ComponentRemovedEvent*>(event);
    if (RequirableComponentIDs.end() == std::find(RequirableComponentIDs.begin(), RequirableComponentIDs.end(),
      castedEvent->RemovedComponentID))
      return;
    
    Entity* owner = castedEvent->Owner;

    auto[positionInCache, isFound] = FindPositionWithID(owner->GetId());
    if (isFound)
    {
        Entities[positionInCache] = nullptr;
        ReusableEntityIds.push(positionInCache);
    }
}

std::tuple<PositionInCache, bool> Group::FindPositionWithID(const EntityID id) const
{
    for (unsigned int i = 0; i < Entities.size(); ++i)
    {
        if (Entities[i] == nullptr)
            continue;

        if (id == Entities[i]->GetId())
            return { i, true };
    }

    return { -1, false };
}
