#pragma once

#include <functional>

class Context;
class Entity;
typedef std::function<void(Context*)> TimerCallback;

namespace Time
{
  Entity* SetupDoOnceTimer(Context* ecsContext, const float secondsBeforeFire, const char* timerName, TimerCallback callback);
}
