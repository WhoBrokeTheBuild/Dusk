#ifndef DUSK_PIPELINE_HPP
#define DUSK_PIPELINE_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/VulkanShader.hpp>
#include <Dusk/NonCopyable.hpp>

#include <Dusk/ThirdParty/vulkan.hpp>

#include <memory>

namespace dusk {

class DUSK_API VulkanPipeline : NonCopyable
{
public:

    using Pointer = std::shared_ptr<VulkanPipeline>;

    VulkanPipeline();
    
    virtual ~VulkanPipeline();

    // TODO: Getters and Setters

    inline void SetTopology(VkPrimitiveTopology topology) {
        _inputAssemblyStateCreateInfo.topology = topology;
    }

    inline void SetPolygonMode(VkPolygonMode polygonMode) {
        _rasterizationStateCreateInfo.polygonMode = polygonMode;
    }

    inline void SetCullMode(VkCullModeFlagBits cullMode) {
        _rasterizationStateCreateInfo.cullMode = cullMode;
    }

    inline void SetFrontFace(VkFrontFace frontFace) {
        _rasterizationStateCreateInfo.frontFace = frontFace;
    }

    bool Create(VulkanShader::Pointer shader);

    void Destroy();

    inline bool Recreate() {
        if (_shader) {
            return Create(_shader);
        }

        return false;
    }

    inline bool CanReload() const {
        return (_shader != nullptr);
    }

    inline VkPipeline GetVkPipeline() {
        return _pipeline;
    }

private:

    VulkanShader::Pointer _shader = nullptr;

    VkPipelineInputAssemblyStateCreateInfo _inputAssemblyStateCreateInfo;

    VkPipelineRasterizationStateCreateInfo _rasterizationStateCreateInfo;

    VkPipelineMultisampleStateCreateInfo _multisampleStateCreateInfo;

    VkPipelineColorBlendAttachmentState _colorBlendAttachmentState;

    VkPipelineColorBlendStateCreateInfo _colorBlendStateCreateInfo;

    VkPipelineDepthStencilStateCreateInfo _depthStencilStateCreateInfo;

    VkPipeline _pipeline = VK_NULL_HANDLE;

}; // VulkanPipeline

} // namespace dusk

#endif // DUSK_PIPELINE_HPP