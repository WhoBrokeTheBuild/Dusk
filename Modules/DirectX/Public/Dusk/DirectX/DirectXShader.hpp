#ifndef DUSK_DIRECTX_SHADER_HPP
#define DUSK_DIRECTX_SHADER_HPP

#include <Dusk/DirectX/DirectXConfig.hpp>

#include <Dusk/Shader.hpp>

#include <optional>

namespace Dusk {

class DUSK_DIRECTX_API DirectXShader : public Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(DirectXShader)

    DirectXShader() = default;

    bool LoadFromFiles(const std::vector<string>& filenameList) override;
    
    D3D12_SHADER_BYTECODE GetShaderBytecode(const ShaderStage& stage);

private:

    bool LoadCSO(const Path& path, bool useAssetPath);

    bool LoadHLSL(const Path& path, bool useAssetPath);

    std::vector<uint8_t> _vertex;

    std::vector<uint8_t> _pixel;

    std::vector<uint8_t> _domain;

    std::vector<uint8_t> _hull;

    std::vector<uint8_t> _geometry;

    std::vector<uint8_t> _compute;

}; // class DirectXShader

DUSK_DIRECTX_API
std::optional<ShaderStage> GetShaderStageFromFilename(string_view filename);

DUSK_DIRECTX_API
std::optional<const wchar_t *> GetShaderEntrypoint(const ShaderStage& stage);

DUSK_DIRECTX_API
std::optional<const wchar_t *> GetShaderTargetProfile(const ShaderStage& stage);

} // namespace Dusk

#endif // DUSK_DIRECTX_SHADER_HPP