#ifndef DUSK_VULKAN_SHADER_HPP
#define DUSK_VULKAN_SHADER_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>
#include <Dusk/Shader.hpp>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_SHADER(x) (dynamic_cast<Dusk::Vulkan::VulkanShader *>(x))

class DUSK_VULKAN_API VulkanShader : public Shader
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanShader)

    VulkanShader() = default;

    virtual ~VulkanShader() = default;

    void Terminate() override;

    bool LoadFromFiles(const std::vector<string>& pathList, bool useAssetPath = true) override;

    inline std::vector<VkPipelineShaderStageCreateInfo>& GetStageList() {
        return _shaderStageList;
    }

private:

    bool LoadSPV(const Path& path, bool useAssetPath);

    VkShaderStageFlagBits GetVkShaderType(const Path& path);

    std::vector<VkShaderModule> _shaderModuleList;

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStageList;

}; // class VulkanShader

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_SHADER_HPP