#ifndef DUSK_PRIMITIVE_HPP
#define DUSK_PRIMITIVE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Material.hpp>
#include <Dusk/PrimitiveData.hpp>

#include <memory>

namespace Dusk {

class DUSK_CORE_API Primitive : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Primitive)

    Primitive() = default;

    virtual ~Primitive() = default;

    virtual void Terminate() { }

    virtual bool Load(const std::unique_ptr<PrimitiveData>& data) = 0;

    virtual inline void SetMaterial(std::shared_ptr<Material> material) {
        _material = material;
    }

    virtual inline std::shared_ptr<Material> GetMaterial() {
        return _material;
    }

protected:

    std::shared_ptr<Material> _material;

}; // class Primitive

} // namespace Dusk

#endif // DUSK_PRIMITIVE_HPP