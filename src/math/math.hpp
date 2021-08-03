#pragma once

#include <array>
#include <cmath>
#include <numbers>

namespace Math
{
  template<class Data>
  class TVec2
  {
  public:
    TVec2()
      : m_Elements{ 0, 0 }
    {
    }

    TVec2(const Data& e0, const Data& e1)
      : m_Elements{ e0, e1 }
    {
    }

    TVec2(const TVec2<Data>& v)
      : m_Elements(v.m_Elements)
    {
    }

    inline const TVec2<Data>& operator=(TVec2<Data> v)
    {
      std::swap(m_Elements, v.m_Elements);
      return *this;
    }

    inline TVec2<Data> operator+(const TVec2<Data>& v) const
    {
      return { m_Elements[0] + v[0], m_Elements[1] + v[1] };
    }

    inline const TVec2<Data>& operator+=(const TVec2<Data>& v)
    {
      m_Elements = operator+(v).m_Elements;
      return *this;
    }

    inline TVec2<Data> operator-(const TVec2<Data>& v) const
    {
      return { m_Elements[0] - v[0], m_Elements[1] - v[1] };
    }

    inline const TVec2<Data>& operator-=(const TVec2<Data>& v)
    {
      m_Elements = operator-(v).m_Elements;
      return *this;
    }

    inline TVec2<Data> operator*(const TVec2<Data>& r) const
    {
      return { x * r.x, y * r.y };
    }

    inline const TVec2<Data>& operator*=(const TVec2<Data>& r)
    {
      m_Elements = operator*(r).m_Elements;
      return *this;
    }

    inline TVec2<Data> operator/(const float v) const
    {
      return { m_Elements[0] / v, m_Elements[1] / v };
    }

    inline TVec2<Data> operator/(const TVec2<Data>& r) const
    {
      return { x / r.x, y / r.y };
    }

    inline TVec2<Data> operator/=(const TVec2<Data>& r) const
    {
      m_Elements = operator/(r).m_Elements;
      return this;
    }

    inline Data& operator[](const size_t n)
    {
      return m_Elements[n];
    }

    inline const Data& operator[](const unsigned int n) const
    {
      return m_Elements[n];
    }

    inline float Length2() const
    {
      const float l = m_Elements[0] * m_Elements[0] + m_Elements[1] * m_Elements[1];
      return l;
    }

    inline float Length() const
    {
      const float l = Length2();
      return std::sqrt(l);
    }

    inline TVec2<Data> Normalize()
    {
      const float l = Length();
      return *this / l;
    }

    inline Data Dot(const TVec2<Data>& r)
    {
      return m_Elements[0] * r[0] + m_Elements[1] * r[1];
    }

  public:
    Data& x = m_Elements[0];
    Data& y = m_Elements[1];

  private:
    std::array<Data, 2> m_Elements;
  };

  template<class Data>
  inline TVec2<Data> operator*(const float l, const TVec2<Data>& r)
  {
    return TVec2(l * r[0], l * r[1]);
  }

  template<class Data>
  inline TVec2<Data> operator*(const TVec2<Data>& l, const float r)
  {
    return operator*(r, l);
  }

  template<class Data>
  class TMat2x2
  {
  public:
    typedef TVec2<Data> Row;

  public:
    TMat2x2(const Data& e0, const Data& e1, const Data& e2, const Data& e3)
      : m_Rows{ {e0, e1}, {e2, e3} }
    {
    }

    TMat2x2(const Data& e)
      : m_Rows{ {e, e}, {e, e} }
    {
    }

    TMat2x2(const TMat2x2<Data>& m)
      : m_Rows{m}
    {
    }

    inline const TMat2x2<Data>& operator=(TMat2x2 r)
    {
      std::swap(m_Rows, r);
      return *this;
    }

    inline Row operator*(const Row& r)
    {
      return Row{
        m_Rows[0].Dot(r),
        m_Rows[1].Dot(r)
      };
    }

    inline Row& operator[](const size_t n_Row)
    {
      return m_Rows[n_Row];
    }

    inline const Row& operator[](const size_t n_Row) const
    {
      return m_Rows[n_Row];
    }

  private:
    std::array<Row, 2> m_Rows;
  };

  typedef TVec2<float> fVec2;
  typedef TVec2<unsigned int> uiVec2;
  typedef TVec2<int> iVec2;

  inline float ToRadians(const float angle)
  {
    return angle * std::numbers::pi_v<float> / 180.0f;
  }

  fVec2 RotateVector(const fVec2 v, const float angle);

  template <typename T> 
  char GetSign(T val) 
  {
    return (T(0) < val) - (val < T(0));
  }

  template <typename T>
  T GetRandom(const T begin, const T end)
  {
    return begin + static_cast<T>(std::rand() % static_cast<int>(end - begin));
  }
}

namespace std
{
  template<class T>
  void swap(Math::TVec2<T>& l, Math::TVec2<T>& r)
  {
    Math::TVec2<T> tmp = l;
    l.x = r.x;
    l.y = r.y;
    r.x = tmp.x;
    r.y = tmp.y;
  }
}