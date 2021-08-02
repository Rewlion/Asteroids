#include "math.hpp"

namespace Math
{
  fVec2 RotateVector(const fVec2 v, const float angle)
  {
    const float r = ToRadians(angle);
    const float sinR = std::sin(r);
    const float cosR = std::cos(r);

    return fVec2{
      v.x * cosR - v.y * sinR,
      v.x * sinR + v.y * cosR
    };
  }
}