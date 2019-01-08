#ifndef DUSK_AXIS_HPP
#define DUSK_AXIS_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Mesh.hpp>

#include <memory>

namespace dusk {

class Axis : public ActorComponent
{
public:

    Axis();

    virtual void Render(RenderContext& ctx) override;

private:

    static inline std::shared_ptr<Mesh> _Mesh = nullptr;

}; // class Axis

} // namespace dusk

#endif // DUSK_AXIS_HPP