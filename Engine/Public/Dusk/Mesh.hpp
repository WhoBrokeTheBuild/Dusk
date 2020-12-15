#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>

#include <vector>

namespace Dusk {

class DUSK_ENGINE_API Mesh : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    virtual ~Mesh() = default;

    virtual void Render() { }

    virtual bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data) = 0;

protected:

    std::vector<std::unique_ptr<Primitive>> _primitives;


}; // class Mesh

std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename);

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP