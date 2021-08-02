#pragma once

#include "input_utility.h"

namespace Gdiplus
{
  class Graphics;
  class Bitmap;
}

class WindowManager
{
public:
  struct SharedWindowProcData
  {
    bool            shouldWindowClose;
    Input::Callback inputCallback;
  };

public:
  struct ApplicationConfig
  {
    void* hInstance;
    wchar_t* pCmdLine;
    int nCmdShow;
    const wchar_t* title;
  };

  struct WindowConfig
  {
    unsigned int width, height;
  };

  struct Config
  {
    ApplicationConfig application;
    WindowConfig window;
    Input::Callback inputCallback;
  };

public:
  WindowManager();
  ~WindowManager();

  void CreateWnd(const Config& config);

  void PollEvents();

  inline void Blt()
  {
   // m_Graphics->DrawImage(m_FrameBitmap, 0, 0); 
  }

  inline void SetInputCallback(Input::Callback callback)
  {
    m_ProcData.inputCallback = callback;
  }

  inline bool ShouldWindowClose() const
  {
    return m_ProcData.shouldWindowClose;
  }

  inline void* GetWindowHandler() const
  {
    return m_WindowHandler;
  }

  inline Gdiplus::Bitmap* GetBitmap()
  {
    return m_FrameBitmap;
  }

  void DrawString(const wchar_t* str);
private:
  Config m_Config;

  unsigned long long m_GdiplusToken;
  void* m_WindowHandler;
  Gdiplus::Graphics* m_Graphics;
  Gdiplus::Bitmap* m_FrameBitmap;

  SharedWindowProcData m_ProcData;
};
