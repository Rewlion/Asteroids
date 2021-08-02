#pragma once

#include <engine/Window/window_manager.h>
#include <engine/ecs/Context.h>

class Engine
{
public:
  struct Settings
  {
    WindowManager::ApplicationConfig application;
    WindowManager::WindowConfig window;
  };

public:
  Engine(const Settings& settings);

  void Start();

  inline void AddSystems(std::function<void(Context&)> addSystems)
  {
    addSystems(m_EcsContext);
  }

private:
  void RunGameLoop(std::function<void(float)> logicStep);

private:
  Settings m_Settings;
  Context m_EcsContext;
  WindowManager m_WindowManager;
};
