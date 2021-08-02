#include "strip_lines_component.h"

std::vector<StripLinesComponent::Point> StripLinesComponent::GetPoints() const
{
  std::vector<StripLinesComponent::Point> ps;
  ps.resize(points.size());

  const Math::fVec2 worldLocation = GetWorldLocation();
  const Math::fVec2 worldSize = GetWorldSize();
  const float worldRotation = GetWorldRotation();

  for (int i = 0; i < ps.size(); ++i)
  {
    Math::fVec2 p = points[i];
    //p *= worldSize;
    //p = Math::RotateVector(p, worldRotation);
    //p += worldLocation;
    p = GetWorldLocation(p);

    ps[i] = p;
  }

  return ps;
}
