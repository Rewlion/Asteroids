#pragma once

enum class Identity : unsigned char
{
  None = 0,
  PlayerShip = 1,
  BigAsteroid = 2,
  MediumAsteroid = 3,
  SmallAsteroid = 4,
  Bullet = 5
};

enum class Team : unsigned char
{
  None = 0,
  Team0 = 1,
  Team1 = 2,
  Team2 = 3
};
