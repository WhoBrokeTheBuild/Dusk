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

    VulkanPipeline() = default;
    
    virtual ~VulkanPipeline();

    // TODO: Getters and Setters

    inline void SetTopology(vk::PrimitiveTopology topology) {
        _inputAssemblyStateCreateInfo.setTopology(topology);
    }

    inline void SetPolygonMode(vk::PolygonMode polygonMode) {
        _rasterizationStateCreateInfo.setPolygonMode(polygonMode);
    }

    inline void SetCullMode(vk::CullModeFlagBits cullMode) {
        _rasterizationStateCreateInfo.setCullMode(cullMode);
    }

    inline void SetFrontFace(vk::FrontFace frontFace) {
        _rasterizationStateCreateInfo.setFrontFace(frontFace);
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

    inline vk::Pipeline GetVkPipeline() {
        return _pipeline;
    }

private:

    VulkanShader::Pointer _shader = nullptr;

    vk::PipelineInputAssemblyStateCreateInfo _inputAssemblyStateCreateInfo = 
        vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(false);

    vk::PipelineRasterizationStateCreateInfo _rasterizationStateCreateInfo = 
        vk::PipelineRasterizationStateCreateInfo()
            .setRasterizerDiscardEnable(false)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.0f)
            .setCullMode(vk::CullModeFlagBits::eNone)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthClampEnable(false)
            .setDepthBiasEnable(false)
            .setDepthBiasClamp(0.0f)
            .setDepthBiasConstantFactor(0.0f)
            .setDepthBiasSlopeFactor(0.0f);

    vk::PipelineMultisampleStateCreateInfo _multisampleStateCreateInfo =
        vk::PipelineMultisampleStateCreateInfo()
            .setRasterizationSamples(vk::SampleCountFlagBits::e1)
            .setSampleShadingEnable(false);

    vk::PipelineColorBlendAttachmentState _colorBlendAttachmentState = 
        vk::PipelineColorBlendAttachmentState()
            .setBlendEnable(false)
            // .setSrcColorBlendFactor(vk::BlendFactor::eSrcColor)
            // .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
            // .setColorBlendOp(vk::BlendOp::eAdd)
            // .setSrcAlphaBlendFactor(vk::BlendFactor::eSrcAlpha)
            // .setDstAlphaBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
            // .setAlphaBlendOp(vk::BlendOp::eAdd)
            .setColorWriteMask(
                vk::ColorComponentFlagBits::eR |
                vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB |
                vk::ColorComponentFlagBits::eA
            );

    vk::PipelineColorBlendStateCreateInfo _colorBlendStateCreateInfo =
        vk::PipelineColorBlendStateCreateInfo()
            .setLogicOpEnable(false)
            .setLogicOp(vk::LogicOp::eCopy)
            .setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f });

    vk::PipelineDepthStencilStateCreateInfo _depthStencilStateCreateInfo =
        vk::PipelineDepthStencilStateCreateInfo()
            .setDepthTestEnable(true)
            .setDepthWriteEnable(true)
            .setDepthCompareOp(vk::CompareOp::eLess)
            .setDepthBoundsTestEnable(false)
            .setStencilTestEnable(false);

    vk::Pipeline _pipeline;

}; // VulkanPipeline

} // namespace dusk

#endif // DUSK_PIPELINE_HPP