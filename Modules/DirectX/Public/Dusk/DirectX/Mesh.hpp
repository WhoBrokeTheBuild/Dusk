#ifndef DUSK_DIRECTX_MESH_HPP
#define DUSK_DIRECTX_MESH_HPP

#include <Dusk/DirectX/Config.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk {

class DUSK_DIRECTX_API DirectXMesh : public Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(DirectXMesh)

    DirectXMesh() = default;

    inline std::string GetClassID() const override {
        return "Dusk::DirectXMesh";
    }

    void Render() override;

    bool Load(const MeshData * data) override;

private:
    
}; // class DirectXMesh

}; // namespace Dusk

#endif // DUSK_DIRECTX_MESH_HPP