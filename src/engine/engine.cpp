#include "Engine.h"

#include <engine/renderer/particles/particles_system.h>
#include <engine/renderer/gdi_render_system.h>
#include <engine/ecs/Context.h>
#include <engine/engine_events.h>
#include <engine/collision/collision_system.h>
#include <engine/utils/time/timer_system.h>
#include <engine/sound/sound_system.h>

Engine::Engine(const Settings& settings)
{
  m_EcsContext.SetUserData(this);
  
  m_WindowManager.CreateWnd(WindowManager::Config{
    .application = settings.application,
    .window = settings.window,
    .inputCallback = [&](Input::Key key, Input::KeyAction action) {
      m_EcsContext.GetEventManager()->SendEvent<InputEvent>(key, action);
    }
  });

  m_EcsContext.GetEventManager()->RegisterEvent<InputEvent>();

  m_EcsContext.AddLogicSystems({
    new Render::GdiRenderSystem(m_WindowManager.GetWindowHandler(), {settings.window.width, settings.window.height}, &m_EcsContext),
    new Render::ParticlesSystem(&m_EcsContext),
    new CollisionSystem(&m_EcsContext),
    new TimerSystem(&m_EcsContext),
    new SoundSystem(&m_EcsContext)
  });
}

void Engine::RunGameLoop(std::function<void(float)> logicStep)
{
  clock_t t1 = clock();
  while (!m_WindowManager.ShouldWindowClose()) {
    m_WindowManager.PollEvents();

    const clock_t t2 = clock();
    const float dt = static_cast<float>(t2 - t1) / static_cast<float>(CLOCKS_PER_SEC);
    t1 = t2;

    logicStep(dt);
  }
}

void Engine::Start()
{
  m_EcsContext.RunInitializationSystems();

  RunGameLoop([&](const float dt) {
    m_EcsContext.UpdateSystems(dt);
  });
}
