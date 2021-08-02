#pragma once

#include <engine/ecs/BaseComponent.h>

enum class GameStatus : unsigned char
{
  None = 0,
  Play = 1,
  Lose = 2,
};

struct GameStatisticsComponent : public BaseComponent
{
  unsigned int playerScore = 0;
  unsigned int playerLifes = 0;
  GameStatus gameStatus = GameStatus::None;
};
