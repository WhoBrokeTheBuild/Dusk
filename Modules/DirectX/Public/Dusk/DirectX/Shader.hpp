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


}; // class Shader

} // namespace Dusk::DirectX

#endif // DUSK_DIRECTX_SHADER_HPP