#pragma once

#include <engine/ecs/BaseComponent.h>

#include <string>

struct SoundRequestLoadComponent : public BaseComponent
{
  std::wstring file;
  std::wstring alias;
};

struct SoundRequestPlayComponent : public BaseComponent
{
  std::wstring alias;
  bool playOnce = false;
};
