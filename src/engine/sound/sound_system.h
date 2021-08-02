#pragma once

#include "sound_event_components.h"

#include <engine/ecs/BaseSystems.h>

#include <thread>
#include <vector>
#include <mutex>

class Group;
class Context;

class SoundSystem : public LogicSystem
{
public:
  SoundSystem(Context* ecsContext);

  virtual void Update(const double dt) override;

private:
  void LoadSounds();
  void PlaySounds();

  void WorkerThread();
  void WorkerLoadSounds();
  void WorkerPlaySounds();

private:
  Group* m_SoundLoadEventsGroup;
  Group* m_SoundPlayEventsGroup;

  std::thread m_WorkerThread;
  std::mutex m_LoadRequestsMutex;
  std::vector<SoundRequestLoadComponent> m_LoadRequests;
  std::mutex m_PlayRequestsMutex;
  std::vector<SoundRequestPlayComponent> m_PlayRequests;

  bool m_ShouldStopWorker;
};