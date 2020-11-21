#ifndef DUSK_DIRECTX_MESH_HPP
#define DUSK_DIRECTX_MESH_HPP

#include <Dusk/DirectX/Config.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk::DirectX {

class DUSK_DIRECTX_API Mesh : public Dusk::Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    explicit Mesh() = default;

    void Render() override;

    bool Load(const MeshData * data) override;

private:
    

}; // class Mesh

}; // namespace Dusk::DirectX

#endif // DUSK_DIRECTX_MESH_HPP