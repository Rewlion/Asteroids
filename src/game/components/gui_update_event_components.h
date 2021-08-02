#pragma once

#include <engine/ecs/BaseComponent.h>

struct GuiUpdateScoreComponent : public BaseComponent
{
  unsigned int newScore = 0;
};

struct GuiUpdatePlayerLifesComponent : public BaseComponent
{
  unsigned char newPlayerLifes = 0.0f;
};
