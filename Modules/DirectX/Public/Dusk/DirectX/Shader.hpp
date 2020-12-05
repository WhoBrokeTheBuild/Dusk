#ifndef DUSK_DIRECTX_SHADER_HPP
#define DUSK_DIRECTX_SHADER_HPP

#include <Dusk/DirectX/Config.hpp>
#include <Dusk/Graphics/Shader.hpp>

namespace Dusk::DirectX {

class DUSK_DIRECTX_API Shader : public Dusk::Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    inline std::string GetClassID() const override {
        return "Dusk::DirectX::Shader";
    }

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind() override;

private:

    enum class Type {
        Unknown = -1,

        Vertex,
        Pixel,
        Domain,
        Hull,
        Geometry,
        Compute,
    };

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

}; // class Shader

} // namespace Dusk::DirectX

#endif // DUSK_DIRECTX_SHADER_HPP