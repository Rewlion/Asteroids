#pragma once

#include <engine/ecs/BaseSystems.h>

class Group;
class Context;

namespace Render
{
  class ParticlesSystem : public LogicSystem
  {
  public:
    ParticlesSystem(Context* ecsContext);

    virtual void Update(const double dt) override;

  private:
    void UpdateDirectEmitters(const float dt);
    void UpdateBurstEmitters(const float dt);

  private:
    Group* m_DirectEmitterGroup;
    Group* m_BurstEmitterGroup;
  };
}