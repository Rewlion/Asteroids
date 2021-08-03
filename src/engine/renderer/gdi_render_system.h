#pragma once

#include <engine/ecs/BaseSystems.h>
#include <engine/renderer/color.h>
#include <math/math.hpp>

#include <functional>

class Group;
struct TextWidgetComponent;

namespace Gdiplus
{
  class Graphics;
  class Bitmap;
  class BitmapData;
}

namespace Render
{
  enum class PixelFormat
  {
    pixel32bppRGB = 0,
    unknown = -1
  };

  class OutputAttachment
  {
  public:
    OutputAttachment();
    OutputAttachment(void* windowHandler, Math::uiVec2 size, PixelFormat format);
    ~OutputAttachment();

    const OutputAttachment& operator=(const OutputAttachment& r) = delete;
    const OutputAttachment& operator=(OutputAttachment&& r);

    void Draw(Gdiplus::Graphics* drawer);
    void Draw(std::function<void(uint32_t*, int, Math::iVec2)> drawCallback);
    void DrawString(const std::wstring& text, const unsigned int size, Math::fVec2 position, Color color);

  private:
    void* m_WindowHandler;
    PixelFormat m_Format;
    Gdiplus::Graphics* m_Graphics;
    Gdiplus::Bitmap* m_Bitmap;
  };

  class GdiRenderSystem : public LogicSystem
  {
  public:
    GdiRenderSystem(void* windowHandler, Math::uiVec2 windowSize, Context* ecsContext);

    virtual void Update(const float dt) override;

  private:
    void ClearRenderTarget(uint32_t* bitmap, int stride, Math::iVec2 size);
    void RenderStripLines(uint32_t* bitmap, int stride, Math::iVec2 size);
    void RenderTextWidgets();
  private:
    void* m_WindowHandler;
    unsigned long long m_GdiplusToken;
    Gdiplus::Graphics* m_WindowGraphics;

    OutputAttachment m_RenderTarget;

    Group* m_StripLinesGroup;
    Group* m_TextWidgetGroup;
  };
}