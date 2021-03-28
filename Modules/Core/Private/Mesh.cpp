#include <Dusk/Mesh.hpp>
#include <Dusk/MeshImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/MediaType.hpp>

namespace Dusk {

DUSK_CORE_API
bool Mesh::Initialize()
{
    bool result = true;

    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    _pipeline = gfx->GetDefaultPipeline();

    _shaderTransformBuffer = gfx->CreateBuffer();
    result = _shaderTransformBuffer->Initialize(
        sizeof(ShaderTransform),
        nullptr,
        BufferUsage::Constant,
        MemoryUsage::UploadOften
    );

    return result;
}

DUSK_CORE_API
void Mesh::Terminate()
{
    if (_shaderTransformBuffer) {
        _shaderTransformBuffer->Terminate();
        _shaderTransformBuffer.reset();
    }
}

DUSK_CORE_API
bool Mesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);
    
    for (const auto& primitiveData : data) {
        auto primitive = gfx->CreatePrimitive();
        if (!primitive || !primitive->Load(primitiveData)) {
            return false;
        }
        _primitiveList.push_back(std::move(primitive));
    }
    
    return true;
}

DUSK_CORE_API
void Mesh::Render(RenderContext * ctx)
{
    if (_shaderTransformBuffer) {
        uint8_t * data = reinterpret_cast<uint8_t *>(ctx->GetShaderTransform());
        _shaderTransformBuffer->WriteTo(0, sizeof(ShaderTransform), data);
    }
}

DUSK_CORE_API
std::shared_ptr<Mesh> Mesh::LoadFromFile(const Path& path, string mediaType /*= ""*/, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    string ext = path.GetExtension();
    
    if (mediaType.empty()) {
        mediaType = GetMediaTypeFromExtension(ext);
    }

    const auto& importers = GetMeshImporterListForMediaType(mediaType);
    for (const auto& importer : importers) {
        const auto& primitiveList = importer->LoadFromFile(path, useAssetPath);
        if (primitiveList.empty()) {
            continue;
        }

        auto mesh = gfx->CreateMesh();
        if (!mesh->Load(primitiveList)) {
            break;
        }

        DuskBenchmarkEnd();
        return mesh;
    }

    LogError(DUSK_ANCHOR, "Failed to load mesh '{}'", path);
    return nullptr;
}

} // namespace Dusk