#pragma once

#include <game/game_objects_types.h>
#include <engine/renderer/strip_lines_component.h>
#include <math/math.hpp>

#include <unordered_map>

namespace Game::Utils
{
  struct GameObjectSettings
  {
    std::vector<StripLinesComponent::Point> model;
    Math::fVec2 size = { 0.0f, 0.0f };
    float circleCollisionRadius = 0.0f;
    unsigned int score = 0;
  };

  const static std::unordered_map<Identity, GameObjectSettings> DefaultGameObjectsSetttings{
    {
      Identity::PlayerShip,
      GameObjectSettings{
        .model = {{-0.5f, -1.0f}, {0.0f, 1.0f}, {0.5f, -1.0f}, {0.0f, -0.3f}, {-0.5f, -1.0f}},
        .size = { 10.0f, 10.0 },
        .circleCollisionRadius = 6.0f,
      }
    },
    {
      Identity::Bullet,
      GameObjectSettings{
        .model = {{-0.5f, -1.0f}, {0.0f, 1.0f}, {0.5f, -1.0f}, {0.0f, -0.3f}, {-0.5f, -1.0f}},
        .size = { 2.0f, 2.0 },
        .circleCollisionRadius = 1.0f,
      }
    },
    {
      Identity::BigAsteroid,
      GameObjectSettings{
        .model = {
          {-1.0f / 4.0f, 1.0f}, {1.0f / 4.0f, 1.0f}, {1.0f / 4.0f, 7.0f / 8.0f},
          {6.0f / 8.0f, 7.0f / 8.0f}, {6.0f / 8.0f, 3.0f / 8.0f}, {1.0f, 3.0f / 8.0f},
          {1.0f, -1.0f / 8.0f}, {7.0f / 8.0f, -1.0f / 8.0f}, {4.0f / 8.0f, -4.0f / 8.0f},
          {4.0f / 8.0f, -1.0f}, {-6.0f / 8.0f, -1.0f}, {-6 / 8, -7 / 8},
          {-1.0f, -6.0f / 8.0f}, {-1.0f, 1.0f / 8.0f}, {-7.0f / 8.0f, 2.0f / 8.0f},
          {-2.0f / 8.0f, 2.0f / 8.0f}, {-1.0f / 4.0f, 1.0f}
        },
        .size = { 40.0f, 40.0 },
        .circleCollisionRadius = 39.0f,
        .score = 10,
      }
    },
    {
      Identity::MediumAsteroid,
      GameObjectSettings{
        .model = {
          {-1.0f / 4.0f, 1.0f}, {1.0f / 4.0f, 1.0f}, {1.0f / 4.0f, 7.0f / 8.0f},
          {6.0f / 8.0f, 7.0f / 8.0f}, {6.0f / 8.0f, 3.0f / 8.0f}, {1.0f, 3.0f / 8.0f},
          {1.0f, -1.0f / 8.0f}, {7.0f / 8.0f, -1.0f / 8.0f}, {4.0f / 8.0f, -4.0f / 8.0f},
          {4.0f / 8.0f, -1.0f}, {-6.0f / 8.0f, -1.0f}, {-6 / 8, -7 / 8},
          {-1.0f, -6.0f / 8.0f}, {-1.0f, 1.0f / 8.0f}, {-7.0f / 8.0f, 2.0f / 8.0f},
          {-2.0f / 8.0f, 2.0f / 8.0f}, {-1.0f / 4.0f, 1.0f}
        },
        .size = { 25.0f, 25.0 },
        .circleCollisionRadius = 24.0f,
        .score = 25,
      }
    },
    {
      Identity::SmallAsteroid,
      GameObjectSettings{
        .model = {
          {-1.0f / 4.0f, 1.0f}, {1.0f / 4.0f, 1.0f}, {1.0f / 4.0f, 7.0f / 8.0f},
          {6.0f / 8.0f, 7.0f / 8.0f}, {6.0f / 8.0f, 3.0f / 8.0f}, {1.0f, 3.0f / 8.0f},
          {1.0f, -1.0f / 8.0f}, {7.0f / 8.0f, -1.0f / 8.0f}, {4.0f / 8.0f, -4.0f / 8.0f},
          {4.0f / 8.0f, -1.0f}, {-6.0f / 8.0f, -1.0f}, {-6 / 8, -7 / 8},
          {-1.0f, -6.0f / 8.0f}, {-1.0f, 1.0f / 8.0f}, {-7.0f / 8.0f, 2.0f / 8.0f},
          {-2.0f / 8.0f, 2.0f / 8.0f}, {-1.0f / 4.0f, 1.0f}
        },
        .size = { 10.0f, 10.0 },
        .circleCollisionRadius = 9.0f,
        .score = 53
      }
    }
  };
}
