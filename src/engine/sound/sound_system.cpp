#include "sound_system.h"
#include "sound_event_components.h"

#include <Windows.h>
#include <engine/ecs/Context.h>

SoundSystem::SoundSystem(Context* ecsContext)
  : LogicSystem(ecsContext)
  , m_SoundLoadEventsGroup(ecsContext->GetGroup<SoundRequestLoadComponent>())
  , m_SoundPlayEventsGroup(ecsContext->GetGroup<SoundRequestPlayComponent>())
  , m_ShouldStopWorker(false)
{
  m_WorkerThread = std::thread(&SoundSystem::WorkerThread, this);
}

void SoundSystem::Update(const double dt)
{
  LoadSounds();
  PlaySounds();
}

void SoundSystem::LoadSounds()
{
  std::lock_guard<std::mutex> lg{ m_LoadRequestsMutex };

  for(Entity* e: m_SoundLoadEventsGroup->GetEntities())
  if (e)
  for (SoundRequestLoadComponent* request: e->GetComponents<SoundRequestLoadComponent>())
  {
    m_LoadRequests.push_back(*request);
    pContext->GetEntityManager()->ReleaseEntity(e);
  }
}

void SoundSystem::PlaySounds()
{
  std::lock_guard<std::mutex> lg{ m_PlayRequestsMutex };

  for(Entity* e: m_SoundPlayEventsGroup->GetEntities())
  if (e)
  for (SoundRequestPlayComponent* request: e->GetComponents<SoundRequestPlayComponent>())
  {
    m_PlayRequests.push_back(*request);
    pContext->GetEntityManager()->ReleaseEntity(e);
  }
}

void SoundSystem::WorkerThread()
{
  while (!m_ShouldStopWorker)
  {
    WorkerLoadSounds();
    WorkerPlaySounds();
  }
}

void SoundSystem::WorkerLoadSounds()
{
  if (!m_LoadRequests.empty())
  {
    std::lock_guard<std::mutex> lg{ m_LoadRequestsMutex };

    for (auto request : m_LoadRequests)
    {
      std::wstring cmd = std::wstring(L"open waveaudio!") + request.file + L" alias " + request.alias;
      mciSendString(cmd.c_str(), NULL, 0, 0);
    }

    m_LoadRequests.clear();
  }
}

void SoundSystem::WorkerPlaySounds()
{
  if (!m_PlayRequests.empty())
  {
    std::lock_guard<std::mutex> lg{ m_PlayRequestsMutex };

    for (auto request : m_PlayRequests)
    {
      std::wstring cmd = std::wstring(L"play ") + request.alias + L" from 0";
      mciSendString(cmd.c_str(), NULL, 0, 0);
    }

    m_PlayRequests.clear();
  }
}
