#ifndef DUSK_DIRECTX_SHADER_HPP
#define DUSK_DIRECTX_SHADER_HPP

#include <Dusk/DirectX/Config.hpp>
#include <Dusk/Graphics/Shader.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <d3d11.h>

namespace Dusk::DirectX {

class DUSK_DIRECTX_API Shader : public Dusk::Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind() override;

private:

    ID3D11VertexShader * _vertShader;
    ID3D11PixelShader * _pixelShader;

}; // class Shader

} // namespace Dusk::DirectX

#endif // DUSK_DIRECTX_SHADER_HPP