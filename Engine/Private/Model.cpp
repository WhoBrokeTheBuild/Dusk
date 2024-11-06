#include <Dusk/Model.hpp>
#include <Dusk/Log.hpp>

#include "glTF.hpp"

namespace dusk {

DUSK_API
void Model::Create(List<Mesh::Pointer>&& meshList)
{
    _meshList = std::move(meshList);

    _transformBuffer.Create(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        sizeof(ShaderTransform)
    );

    for (auto& mesh : _meshList) {
        mesh->UpdateDescriptorSet(&_transformBuffer);
    }
}

DUSK_API
bool Model::LoadFromFile(const Path& path)
{
    Free();

    Log(DUSK_ANCHOR, "Loading {}", path.string());

    auto assetPathList = GetAssetPathList();

    auto extension = path.extension();
    if (extension == ".glb" || extension == ".gltf") {
        glTF::Loader file;
        if (path.is_absolute()) {
            if (file.LoadFromFile(path)) {
                _path = path;
            }
        }
        else {
            for (const auto& assetPath : assetPathList) {
                auto fullPath = assetPath / path;
                Log(DUSK_ANCHOR, "Checking {}", fullPath.string());
                if (file.LoadFromFile(fullPath)) {
                    _path = path;
                    break;
                }
            }
        }

        if (_path.empty()) {
            return false;
        }

        Create(std::move(file.MeshList));
    }

    return true;
}

DUSK_API
void Model::Free()
{
    _meshList.clear();
}

DUSK_API
void Model::Update(mat4 view, mat4 proj)
{
    ShaderTransform shaderTransform;
    shaderTransform.Model = Transform.ToMatrix();
    shaderTransform.View = view;
    shaderTransform.Projection = proj;
    shaderTransform.UpdateMVP();

    _transformBuffer.WriteTo(0, sizeof(shaderTransform), &shaderTransform);
}

} // namespace dusk