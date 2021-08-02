#pragma once

#include <engine/scene_component.h>
#include <engine/renderer/color.h>
#include <math/math.hpp>

#include <vector>

struct StripLinesComponent : public SceneComponent
{
  typedef Math::fVec2 Point;

  std::vector<Point> GetPoints() const;

  inline void SetPoints(const std::vector<Point>& points)
  {
    this->points = points;
  }

  inline void SetColor(const Color color)
  {
    this->color = color;
  }

  inline Color GetColor() const
  {
    return color;
  }

private:
  std::vector<Point> points;
  Color color = Color::Chocolate;
};
