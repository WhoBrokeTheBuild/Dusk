#ifndef DUSK_CORE_MESH_HPP
#define DUSK_CORE_MESH_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>
#include <Dusk/Pipeline.hpp>
#include <Dusk/RenderContext.hpp>
#include <Dusk/Buffer.hpp>
#include <Dusk/String.hpp>

#include <vector>

namespace Dusk {

class DUSK_CORE_API Mesh : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    static std::shared_ptr<Mesh> LoadFromFile(const Path& filename, string mediaType = "", bool useAssetPath = true);

    Mesh() = default;

    virtual ~Mesh() = default;

    virtual bool Initialize();

    virtual void Terminate();

    virtual bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data);

    virtual void Render(RenderContext * ctx);

    virtual void SetPipeline(std::shared_ptr<Pipeline> pipeline) {
        _pipeline = pipeline;
    }

protected:

    std::vector<std::shared_ptr<Primitive>> _primitiveList;

    std::shared_ptr<Pipeline> _pipeline;

    std::shared_ptr<Buffer> _shaderTransformBuffer;

}; // class Mesh

} // namespace Dusk

#endif // DUSK_CORE_MESH_HPP