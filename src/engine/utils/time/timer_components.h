#pragma once

#include <engine/ecs/BaseComponent.h>

#include <functional>

class Context;
typedef std::function<void(Context*)> TimerCallback;

struct DoOnceTimerComponent : public BaseComponent
{
  float secondsBeforeFire = 0.0f;
  float tickedTime = 0.0f;
  TimerCallback callback;
};

