#include <Dusk/DirectX/Mesh.hpp>
#include <Dusk/DirectX/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>

#include <d3d11.h>

namespace Dusk::DirectX {

DUSK_DIRECTX_API
void Mesh::Render()
{

}

DUSK_DIRECTX_API
bool Mesh::Load(const MeshData * data)
{
    DuskBenchmarkStart();

    const auto& indices = data->GetIndices();
    const auto& vertices = data->GetVertices();
    const auto& normals = data->GetNormals();
    const auto& uvs = data->GetUVs();
    const auto& colors = data->GetColors();

    DuskBenchmarkEnd("DirectX::Mesh::Load");
    return true;
}

}; // namespace Dusk::DirectX