#pragma once

#include <engine/ecs/BaseComponent.h>
#include <engine/renderer/color.h>
#include <math/math.hpp>

#include <string>

struct TextWidgetComponent : public BaseComponent
{
  std::wstring text;
  Math::fVec2 position = { 0.0f, 0.0f };
  unsigned char size = 7;
  Color color = Color::Chocolate;
};
