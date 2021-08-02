#pragma once

#include <string>

#include <engine/ecs/Context.h>
#include "sound_event_components.h"

namespace Sound
{
  inline void LoadSound(Context* ecsContext, const std::wstring& file, const std::wstring& alias)
  {
    auto* request = ecsContext->GetEntityManager()->NewEntity()->AddComponent<SoundRequestLoadComponent>("load sound request");
    request->file = file;
    request->alias = alias;
  }

  inline void PlaySound(Context* ecsContext, const std::wstring& alias, const bool playOnce = true)
  {
    auto* request = ecsContext->GetEntityManager()->NewEntity()->AddComponent<SoundRequestPlayComponent>("play sound request");
    request->alias = alias;
    request->playOnce = playOnce;
  }
}
