#include <engine/Engine.h>
#include <game/systems/level_initialize_system.h>
#include <game/systems/player_input_system.h>
#include <game/systems/movement_system.h>
#include <game/systems/field_border_teleport_system.h>
#include <game/systems/game_objects_spawn_system.h>
#include <game/systems/death_clock_system.h>
#include <game/systems/asteroids_spawn_system.h>
#include <game/systems/player_respawn_system.h>
#include <game/systems/damage_system.h>
#include <game/systems/gui_system.h>

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  const unsigned int width = 800;
  const unsigned int height = 600;
  
  Engine engine{ Engine::Settings{
    .application = {
      .hInstance = hInstance,
      .pCmdLine = pCmdLine,
      .nCmdShow = nCmdShow,
      .title = L"Asteroids"
    },
    .window = {
      .width = width,
      .height = height,
    },
  }};

  engine.AddSystems([](Context& ecsContext) {

    ecsContext.AddInitializationSystems({
      new LevelInitializeSystem{&ecsContext},
    });

    ecsContext.AddLogicSystems({
      new GameObjectsSpawnSystem{&ecsContext},
      new Input::PlayerInputSystem{&ecsContext},
      new MovementSystem{&ecsContext},
      new FieldBorderTeleportSystem{&ecsContext},
      new DeathClockSystem{&ecsContext},
      new AsteroidsSpawnSystem{&ecsContext},
      new PlayerRespawnSystem{&ecsContext},
      new DamageSystem{&ecsContext},
      new GuiSystem{&ecsContext},
    });
  });

  engine.Start();
}
