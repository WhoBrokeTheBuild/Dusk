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

    bool LoadFromFiles(const std::vector<string>& pathList, bool useAssetPath = true) override;
    
    D3D12_SHADER_BYTECODE GetShaderBytecode(ShaderStage stage);

private:

    bool LoadCSO(const Path& path, ShaderStage stage, const string& entrypoint, bool useAssetPath);

    bool LoadHLSL(const Path& path, ShaderStage stage, const string& entrypoint, bool useAssetPath);

    std::unordered_map<ShaderStage, std::vector<uint8_t>> _bytecodeByStage;

}; // class DirectXShader

} // namespace Dusk

#endif // DUSK_DIRECTX_SHADER_HPP