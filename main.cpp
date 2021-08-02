#include "window_manager.h"

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  WindowManager wndMgr;
  wndMgr.CreateWnd(WindowManager::Config{
    .hInstance = hInstance,
    .pCmdLine = pCmdLine,
    .nCmdShow = nCmdShow,
    .title = L"Asteroids",
    .width = 800,
    .height = 600,
    .inputCallback = [](Input::Key k, Input::KeyAction a) {
    }
  });

  while (!wndMgr.ShouldWindowClose())
    wndMgr.PollEvents();

}
