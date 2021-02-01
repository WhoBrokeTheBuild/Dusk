#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>
#include <Dusk/Pipeline.hpp>
#include <Dusk/RenderContext.hpp>
#include <Dusk/Buffer.hpp>

#include <vector>

namespace Dusk {

class DUSK_ENGINE_API Mesh : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    virtual ~Mesh() = default;

    virtual bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data);

    virtual void Render(RenderContext * ctx) = 0;

    virtual void SetPipeline(std::shared_ptr<Pipeline> pipeline) {
        _pipeline = pipeline;
    }

protected:

    std::vector<std::unique_ptr<Primitive>> _primitiveList;

    std::shared_ptr<Pipeline> _pipeline;

}; // class Mesh

DUSK_ENGINE_API
std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename, bool useAssetPath = true);

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP