#ifndef DUSK_PRIMITIVE_DATA_HPP
#define DUSK_PRIMITIVE_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Primitive.hpp>

#include <gsl/gsl>

namespace Dusk {

class DUSK_ENGINE_API PrimitiveData : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(PrimitiveData);

    PrimitiveData() = default;

    virtual ~PrimitiveData() = default;

    virtual PrimitiveTopology GetTopology() const = 0;

    virtual gsl::span<uint32_t> GetIndexList() {
        return gsl::span<uint32_t>();
    }
    
    virtual gsl::span<Vertex> GetVertexList() = 0;

    virtual std::shared_ptr<Material> GetMaterial() {
        return nullptr;
    }

    virtual void CalculateTangents();

}; // class PrimitiveData

} // namespace Dusk

#endif // DUSK_PRIMITIVE_DATA_HPP