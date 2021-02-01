#include <Dusk/Mesh.hpp>
#include <Dusk/MeshImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>

namespace Dusk {

DUSK_ENGINE_API
bool Mesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    bool result;

    GraphicsDriver * gfx = GetGraphicsDriver();
    
    for (const auto& primitiveData : data) {
        auto primitive = gfx->CreatePrimitive();
        if (!primitive || !primitive->Load(primitiveData)) {
            return false;
        }
        _primitiveList.push_back(std::move(primitive));
    }
    
    _shaderTransformBuffer = gfx->CreateBuffer();
    result = _shaderTransformBuffer->Initialize(
        sizeof(ShaderTransform),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    return result;
}

DUSK_ENGINE_API
std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& filename, bool useAssetPath /*= true*/)
{
    GraphicsDriver * gfx = GetGraphicsDriver();

    const auto& importers = GetAllMeshImporters();
    for (const auto& importer : importers) {
        const auto& primitiveList = importer->LoadFromFile(filename, useAssetPath);
        if (primitiveList.empty()) {
            continue;
        }

        auto mesh = gfx->CreateMesh();
        if (!mesh->Load(primitiveList)) {
            break;
        }

        return mesh;
    }

    DuskLogError("Failed to load mesh '%s'", filename);
    return nullptr;
}

void Mesh::Render(RenderContext * ctx)
{
    uint8_t * data = reinterpret_cast<uint8_t *>(ctx->GetShaderTransform());
    _shaderTransformBuffer->WriteTo(0, sizeof(ShaderTransform), data);
}

} // namespace Dusk