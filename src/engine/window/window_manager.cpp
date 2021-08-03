#include "window_manager.h"

#include <assert.h>
#include <Windows.h>
#include <Gdiplus.h>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
#include <gdipluscolor.h>
#include <gdiplusinit.h>
#include <gdiplusimaging.h>
#include <Gdipluspixelformats.h>

namespace
{
  LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    auto* procData = reinterpret_cast<WindowManager::SharedWindowProcData*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    const bool isExtended = (lParam & 0x01000000) != 0;

    switch (uMsg)
    {
    case WM_DESTROY:
    {
      procData->shouldWindowClose = true;
      return 0;
    }

    case WM_KEYUP:
    case WM_KEYDOWN:
    {
      if (!procData || !procData->inputCallback)
        return -1;

      static Input::KeyAction lastKeysAction[256]{ Input::KeyAction::Release };

      const auto action = uMsg == WM_KEYDOWN ? Input::KeyAction::Press : Input::KeyAction::Release;
      const unsigned int scanCode = static_cast<unsigned int>(wParam);
      const Input::Key key = Input::ConvertWinApiKeyToManagerKey(scanCode, isExtended);

      if (action != lastKeysAction[wParam])
      {
        lastKeysAction[wParam] = action;
        procData->inputCallback(key, action);
      }

      return 0;
    }

    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
  }
}

void WindowManager::CreateWnd(const Config& config)
{
  const LPCWSTR CLASS_NAME = L"Sample Window Class";

  HINSTANCE hInst = reinterpret_cast<HINSTANCE>(config.application.hInstance);
  WNDCLASS wc = { };
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInst;
  wc.lpszClassName = CLASS_NAME;

  RegisterClass(&wc);

  RECT rect;
  rect.left = 0;
  rect.right = config.window.width;
  rect.top = 0;
  rect.bottom = config.window.height;

  AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

  m_WindowHandler = CreateWindowEx(
    0,                              // Optional window styles.
    CLASS_NAME,                     // Window class
    config.application.title,    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    rect.right - rect.left,
    rect.bottom - rect.top,

    NULL,       // Parent window
    NULL,       // Menu
    hInst,
    NULL        // Additional application data
  );

  assert(m_WindowHandler != 0);
  ShowWindow(reinterpret_cast<HWND>(m_WindowHandler), config.application.nCmdShow);

  m_ProcData.inputCallback = config.inputCallback;
  SetWindowLongPtrA(reinterpret_cast<HWND>(m_WindowHandler), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_ProcData));
}

WindowManager::WindowManager()
  : m_ProcData{ false, nullptr }
{
  
}

WindowManager::~WindowManager()
{
  Gdiplus::GdiplusShutdown(m_GdiplusToken);
}

void WindowManager::PollEvents()
{
  MSG msg = { };
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void WindowManager::DrawString(const wchar_t* str)
{
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(reinterpret_cast<HWND>(m_WindowHandler), &ps);

  // Initialize arguments.
  Gdiplus::Font myFont(L"Arial", 16);
  Gdiplus::RectF layoutRect(0.0f, 0.0f, 200.0f, 50.0f);
  Gdiplus::StringFormat format;
  format.SetAlignment(Gdiplus::StringAlignmentCenter);
  Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 255, 255, 255));

  // Draw string.
  m_Graphics->DrawString(
    str,
    11,
    &myFont,
    layoutRect,
    &format,
    &blackBrush);

  EndPaint(reinterpret_cast<HWND>(m_WindowHandler), &ps);
}