#pragma once

#include <engine/ecs/BaseComponent.h>

#include <game/game_objects_types.h>

struct IdentityComponent : public BaseComponent
{
  Identity identity = Identity::None;
  Team team = Team::None;
};
