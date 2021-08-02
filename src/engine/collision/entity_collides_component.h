#pragma once

#include <engine/ecs/BaseComponent.h>

#include <vector>

class Entity;

struct EntityCollidesComponent : public BaseComponent
{
  std::vector<Entity*> entitiesCollidedWith;
};
