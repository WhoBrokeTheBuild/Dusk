#ifndef DUSK_AXIS_HPP
#define DUSK_AXIS_HPP

#include <dusk/asset/Mesh.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>

namespace dusk {

class Axis : public ActorComponent
{
public:

    Axis();

    void Render(RenderContext&) override;

private:

    static inline std::shared_ptr<Mesh> _Mesh = nullptr;

}; // class Axis

} // namespace dusk

#endif // DUSK_AXIS_HPP
