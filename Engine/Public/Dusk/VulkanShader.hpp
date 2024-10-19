#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/List.hpp>
#include <Dusk/Tuple.hpp>
#include <Dusk/NonCopyable.hpp>
#include <Dusk/String.hpp>
#include <Dusk/VulkanTexture.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanShader : NonCopyable
{
public:

    using Pointer = std::shared_ptr<VulkanShader>;

    VulkanShader() = default;
    
    virtual ~VulkanShader();

    bool LoadFromFiles(const List<Path>& pathList);

    void Destroy();

    bool Reload();

    inline bool CanReload() const {
        return not _pathList.empty();
    }

    inline List<VkPipelineShaderStageCreateInfo>& GetPipelineShaderStageCreateInfoList() {
        return _pipelineShaderStageCreateInfoList;
    }

    // inline VkPipelineLayout& GetPipelineLayout() {
    //     return _pipelineLayout;
    // }

    void GenerateCommands(VkCommandBuffer commandBuffer);

    void SetTexture(StringView name, VulkanTexture *);

private:

    bool loadSPV(const Path& path);

    List<Path> _pathList;

    List<SpvReflectShaderModule> _reflectShaderModuleList;

    List<VkShaderModule> _shaderModuleList;

    List<VkPipelineShaderStageCreateInfo> _pipelineShaderStageCreateInfoList;

    List<VkPushConstantRange> _pushConstantRangeList;

    // List<VkDescriptorSetLayout> _descriptorSetLayoutList;

    // VkPipelineLayout _pipelineLayout;

}; // class VulkanShader

} // namespace dusk

#endif // DUSK_SHADER_HPP