#pragma once

#include <engine/ecs/BaseComponent.h>
#include <math/math.hpp>
#include <assert.h>

struct SceneComponent : BaseComponent
{
public:
  inline void Detach()
  {
    parent = nullptr;
  }

  inline void AttachTo(const SceneComponent* parent)
  {
    assert(parent != nullptr);
    this->parent = parent;
  }

  inline void SetLocalLocation(const Math::fVec2& location)
  {
    this->location = location;
  }

  inline Math::fVec2 GetLocalLocation() const
  {
    return location;
  }

  inline Math::fVec2 GetWorldLocation() const
  {
    const SceneComponent* parent = this;

    Math::fVec2 r = { 0.0f, 0.0f };

    while (parent != nullptr)
    {
      r *= parent->size;
      r = Math::RotateVector(r, parent->rotation);
      r += parent->location;

      parent = parent->parent;
    }

    return r;
  }

  inline Math::fVec2 GetLocalSize() const
  {
    return size;
  }

  inline Math::fVec2 GetWorldSize() const
  {
    const SceneComponent* parent = this;
    Math::fVec2 size = { 0.0f, 0.0f };

    while (parent != nullptr)
    {
      size += parent->size;
      parent = parent->parent;
    }

    return size;
  }

  inline void SetLocalSize(const Math::fVec2 size)
  {
    this->size = size;
  }

  inline float GetLocalRotation() const
  {
    return rotation;
  }

  inline float GetWorldRotation() const
  {
    const SceneComponent* parent = this;
    float rotation = 0.0f;

    while (parent != nullptr)
    {
      rotation += parent->rotation;
      parent = parent->parent;
    }

    return rotation;
  }

  inline Math::fVec2 GetWorldLocation(Math::fVec2 r) const
  {
    const SceneComponent* parent = this;

    while (parent != nullptr)
    {
      r *= parent->size;
      r = Math::RotateVector(r, parent->rotation);
      r += parent->location;

      parent = parent->parent;
    }

    return r;
  }

  inline void SetLocalRotation(const float rotation)
  {
    this->rotation = rotation;
  }

  inline Math::fVec2 GetForward()
  {
    return Math::RotateVector({ 0.0f, 1.0f }, rotation);
  }

  inline Math::fVec2 GetBackward()
  {
    return Math::RotateVector({ 0.0f, -1.0f }, rotation);
  }

protected:
  Math::fVec2 location = { 0.0f, 0.0f };
  Math::fVec2 size = { 1.0f, 1.0f };
  float rotation = 0.0f;
  const SceneComponent* parent = nullptr;
};
