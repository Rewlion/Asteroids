#include "gdi_render_system.h"

#include <engine/renderer/color.h>
#include <engine/renderer/strip_lines_component.h>
#include <engine/renderer/text_widget_component.h>
#include <engine/ecs/Context.h>

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
  Gdiplus::PixelFormat ConvertPixelFormatToGdi(Render::PixelFormat format)
  {
    switch (format)
    {
    case Render::PixelFormat::pixel32bppRGB:
      return PixelFormat32bppRGB;
    default:
      assert(!"unsupported pixel format");
    }

    return -1;
  }

  void DrawLine(uint32_t* bitmap, int stride, Math::iVec2 size, StripLinesComponent::Point p1, StripLinesComponent::Point p2, RGB color)
  {
    Math::iVec2 a{static_cast<int>(p1.x), static_cast<int>(p1.y) };
    Math::iVec2 b{ static_cast<int>(p2.x), static_cast<int>(p2.y) };

    if (a.x == b.x || a.y == b.y)
    {
      bool swapped = false;
      if (a.x == b.x)
      {
        swapped = true;
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
        std::swap(size.x, size.y);
      }

      if (a.y >= 0 && a.y < size.y)
        for (int x = a.x, y = a.y; x <= b.x; ++x)
          if (x >= 0 && x < size.x)
          {
            const size_t i = swapped ? (x * stride / 4 + y)
                                     : (y * stride / 4 + x);

            bitmap[i] = static_cast<uint32_t>(color);
          }

      return;
    }
    else
    {
      bool swapped = false;
      if (std::abs(a.y - b.y) > std::abs(a.x - b.x))
      {
        swapped = true;
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
        std::swap(size.x, size.y);
      }

      if (a.x > b.x)
        std::swap(a, b);

      const int dy = b.y - a.y;
      const int dx = b.x - a.x;

      if (a.x != b.x)
      {
        const int sy = b.y > a.y ? 1 : -1;
        int y = a.y;
        int d = 0;

        for (int x = a.x; x < b.x; ++x)
        {
          d += 2 * std::abs(dy);

          if (d > dx)
          {
            y += sy;
            d -= 2 * dx;
          }

          if (x >= 0 && y >= 0 && x < size.x && y < size.y)
          {
            const size_t i = swapped ? (x * stride / 4 + y)
                                     : (y * stride / 4 + x);

            bitmap[i] = static_cast<uint32_t>(color);
          }
        }
      }
    }
  }
}

namespace Render
{
  OutputAttachment::OutputAttachment()
    : m_WindowHandler(nullptr)
    , m_Format(PixelFormat::unknown)
    , m_Bitmap(nullptr)
    , m_Graphics(nullptr)
  {
  }

  OutputAttachment::OutputAttachment(void* windowHandler, Math::uiVec2 size, PixelFormat format)
  {
    assert(format == PixelFormat::pixel32bppRGB);

    m_WindowHandler = windowHandler;
    m_Bitmap = new Gdiplus::Bitmap(size.x, size.y, ConvertPixelFormatToGdi(format));
    m_Graphics = Gdiplus::Graphics::FromImage(m_Bitmap);
    m_Format = format;
  }

  OutputAttachment::~OutputAttachment()
  {
    delete m_Bitmap;
    delete m_Graphics;
  }

  const OutputAttachment& OutputAttachment::operator=(OutputAttachment&& r)
  {
    m_WindowHandler = r.m_WindowHandler; r.m_WindowHandler = nullptr;
    m_Bitmap = r.m_Bitmap; r.m_Bitmap = nullptr;
    m_Graphics = r.m_Graphics; r.m_Graphics = nullptr;
    m_Format = r.m_Format; r.m_Format = PixelFormat::unknown;

    return *this;
  }

  void OutputAttachment::Draw(Gdiplus::Graphics* drawer)
  {
    drawer->DrawImage(m_Bitmap, 0, 0);
  }

  void OutputAttachment::Draw(std::function<void(uint32_t*, int, Math::iVec2)> drawCallback)
  {
    Gdiplus::BitmapData bd;
    auto size = Gdiplus::Rect{ 0,0,static_cast<INT>(m_Bitmap->GetWidth()), static_cast<INT>(m_Bitmap->GetHeight()) };

    m_Bitmap->LockBits(&size, Gdiplus::ImageLockModeWrite, ConvertPixelFormatToGdi(m_Format), &bd);
    uint32_t* bitmap = reinterpret_cast<uint32_t*>(bd.Scan0);
    drawCallback(bitmap, bd.Stride, { size.Width, size.Height });
    m_Bitmap->UnlockBits(&bd);
  }

  void OutputAttachment::DrawString(const std::wstring& text, const unsigned int size, Math::fVec2 position, Color color)
  {
    Gdiplus::Font myFont(L"Arial", static_cast<Gdiplus::REAL>(size));
    Gdiplus::RectF layoutRect(position.x, position.y, 400.0f, 50.0f);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::SolidBrush blackBrush(static_cast<uint32_t>(color));

    m_Graphics->DrawString(
      text.c_str(),
      static_cast<INT>(text.size()),
      &myFont,
      layoutRect,
      &format,
      &blackBrush);
  }

  GdiRenderSystem::GdiRenderSystem(void* windowHandler, Math::uiVec2 windowSize, Context* ecsContext)
    : LogicSystem(ecsContext)
    , m_WindowHandler(windowHandler)
    , m_StripLinesGroup(ecsContext->GetGroup<StripLinesComponent>())
    , m_TextWidgetGroup(ecsContext->GetGroup<TextWidgetComponent>())
  {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(reinterpret_cast<ULONG_PTR*>(&m_GdiplusToken), &gdiplusStartupInput, NULL);

    const HDC deviceDC = GetDC(reinterpret_cast<HWND>(m_WindowHandler));
    m_WindowGraphics = Gdiplus::Graphics::FromHDC(deviceDC);

    m_RenderTarget = OutputAttachment(windowHandler, windowSize, PixelFormat::pixel32bppRGB);
  }

  void GdiRenderSystem::Update(const float dt)
  {
    m_RenderTarget.Draw([&](uint32_t* bitmap, int stride, Math::iVec2 size)
    {
      ClearRenderTarget(bitmap, stride, size);
      RenderStripLines(bitmap, stride, size);
    });

    RenderTextWidgets();

    m_RenderTarget.Draw(m_WindowGraphics);
  }

  void GdiRenderSystem::ClearRenderTarget(uint32_t* bitmap, int stride, Math::iVec2 size)
  {
    for (int x = 0; x < size.x; ++x)
      for (int y = 0; y < size.y; ++y)
      {
        bitmap[y * stride / 4 + x] = static_cast<uint32_t>(Color::Black);
      }
  }

  void GdiRenderSystem::RenderStripLines(uint32_t* bitmap, int stride, Math::iVec2 size)
  {
    for (Entity* e : m_StripLinesGroup->GetEntities())
    if (e)
    for (auto* stripLineComponent : e->GetComponents<StripLinesComponent>())
    {
      std::vector<StripLinesComponent::Point> points = stripLineComponent->GetPoints();
      if (points.size() >= 2)
        for (int i = 0; i < points.size() - 1; ++i)
          DrawLine(bitmap, stride, size, points[i], points[i + 1], stripLineComponent->GetColor());
    }
  }

  void GdiRenderSystem::RenderTextWidgets()
  {
    for (Entity* e: m_TextWidgetGroup->GetEntities())
    if (e)
    for (TextWidgetComponent* widget: e->GetComponents<TextWidgetComponent>())
    {
      m_RenderTarget.DrawString(widget->text, widget->size, widget->position, widget->color);
    }
  }
}

