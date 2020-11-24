#ifndef DUSK_VULKAN_SHADER_HPP
#define DUSK_VULKAN_SHADER_HPP

#include <Dusk/Vulkan/Config.hpp>
#include <Dusk/Graphics/Shader.hpp>

namespace Dusk::Vulkan {

class DUSK_VULKAN_API Shader : public Dusk::Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(Shader)

    Shader() = default;

    virtual ~Shader() = default;

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind() override;

private:
    VkShaderStageFlagBits GetVkShaderType(const std::string& filename);

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

}; // class Shader

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_SHADER_HPP