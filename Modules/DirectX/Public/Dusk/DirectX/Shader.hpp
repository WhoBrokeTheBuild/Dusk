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

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind() override;

private:

    bool LoadCSO(const std::string& filename);

    bool LoadHLSL(const std::string& filename);

    const wchar_t * GetEntrypoint(const std::string& filename);

    const wchar_t * GetTargetProfile(const std::string& filename);

}; // class Shader

} // namespace Dusk::DirectX

#endif // DUSK_DIRECTX_SHADER_HPP