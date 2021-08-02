#pragma once

#include <engine/ecs/Events/BaseEvent.h>
#include <engine/window/input_utility.h>

struct InputEvent : public BaseEvent
{
  typedef std::shared_ptr<InputEvent> Ptr;

  InputEvent(Input::Key key, Input::KeyAction action)
    : key(key)
    , action(action)
  {}

  Input::Key key;
  Input::KeyAction action;
};
