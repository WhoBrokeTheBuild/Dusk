#ifndef DUSK_DIRECTX_SHADER_HPP
#define DUSK_DIRECTX_SHADER_HPP

#include <Dusk/DirectX/DirectXConfig.hpp>

#include <Dusk/Shader.hpp>

namespace Dusk {

class DUSK_DIRECTX_API DirectXShader : public Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(DirectXShader)

    DirectXShader() = default;

    inline std::string GetClassID() const override {
        return "DuskDirectX::Shader";
    }

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

private:

    bool LoadCSO(const std::string& filename);

    bool LoadHLSL(const std::string& filename);

    Type GetType(const std::string& filename) const;

    const wchar_t * GetEntrypoint(const Type& type) const;

    const wchar_t * GetTargetProfile(const Type& type) const;
    
    ComPtr<IDxcBlob>& GetBlob(const Type& type);

    ComPtr<IDxcBlob> _vs; // Vertex Shader
    ComPtr<IDxcBlob> _ps; // Pixel Shader
    ComPtr<IDxcBlob> _ds; // Domain Shader
    ComPtr<IDxcBlob> _hs; // Hull Shader
    ComPtr<IDxcBlob> _gs; // Geometry Shader
    ComPtr<IDxcBlob> _cs; // Compute Shader

}; // class DirectXShader

} // namespace Dusk

#endif // DUSK_DIRECTX_SHADER_HPP