#ifndef DUSK_VULKAN_SHADER_HPP
#define DUSK_VULKAN_SHADER_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Shader.hpp>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_SHADER(x) (dynamic_cast<Dusk::Vulkan::VulkanShader *>(x))

class DUSK_VULKAN_API VulkanShader : public Dusk::Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanShader)

    VulkanShader() = default;

    virtual ~VulkanShader() = default;

    inline std::string GetClassID() const override {
        return "Dusk::Shader";
    }

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    inline std::vector<VkPipelineShaderStageCreateInfo>& GetStages() {
        return _shaderStages;
    }

private:

    bool LoadSPV(const std::string& filename);

    VkShaderStageFlagBits GetVkShaderType(const std::string& filename);

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;

}; // class VulkanShader

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_SHADER_HPP