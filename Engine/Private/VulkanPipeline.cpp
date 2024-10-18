#include <Dusk/VulkanPipeline.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/Array.hpp>
#include <Dusk/PrimitiveVertex.hpp>

namespace dusk {

DUSK_API
VulkanPipeline::~VulkanPipeline()
{
    Destroy();
}

DUSK_API
bool VulkanPipeline::Create(VulkanShader::Pointer shader)
{
    Destroy();

    _shader = shader;

    auto stageList = _shader->GetPipelineShaderStageCreateInfoList();
    // auto pipelineLayout = _shader->GetPipelineLayout();

    Array<vk::VertexInputBindingDescription, 1> vertexInputBindingDescriptionList = {
        vk::VertexInputBindingDescription()
            .setBinding(0)
            .setStride(sizeof(PrimitiveVertex))
            .setInputRate(vk::VertexInputRate::eVertex)
    };

    Array<vk::VertexInputAttributeDescription, 7> vertexInputAttributeDescriptionList = {
        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::Position)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32B32A32Sfloat)
            .setOffset(offsetof(PrimitiveVertex, Position)),

        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::Normal)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32B32A32Sfloat)
            .setOffset(offsetof(PrimitiveVertex, Normal)),

        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::Tangent)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32B32A32Sfloat)
            .setOffset(offsetof(PrimitiveVertex, Tangent)),

        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::Color)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32B32A32Sfloat)
            .setOffset(offsetof(PrimitiveVertex, Color)),

        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::TexCoord)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32Sfloat)
            .setOffset(offsetof(PrimitiveVertex, TexCoord)),

        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::Joints)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32B32A32Uint)
            .setOffset(offsetof(PrimitiveVertex, Joints)),

        vk::VertexInputAttributeDescription()
            .setLocation(PrimitiveVertex::AttributeLocation::Weights)
            .setBinding(0)
            .setFormat(vk::Format::eR32G32B32A32Sfloat)
            .setOffset(offsetof(PrimitiveVertex, Weights)),
    };

    auto viewportStateCreateInfo = vk::PipelineViewportStateCreateInfo()
        .setViewportCount(1)
        .setScissorCount(1);

    Array<vk::DynamicState, 2> dynamicStateList = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
        // vk::DynamicState::ePrimitiveTopology,
    };

    // TODO: eDepthBias, eStencilCompareMask, eStencilReference, eStencilWriteMask

    auto dynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo()
        .setDynamicStates(dynamicStateList);

    auto vertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo()
        .setVertexBindingDescriptions(vertexInputBindingDescriptionList)
        .setVertexAttributeDescriptions(vertexInputAttributeDescriptionList);

    _colorBlendStateCreateInfo.setAttachments({ _colorBlendAttachmentState });

    auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo()
        .setStages(stageList)
        .setPVertexInputState(&vertexInputStateCreateInfo)
        .setPInputAssemblyState(&_inputAssemblyStateCreateInfo)
        // .setPTessellationState()
        .setPViewportState(&viewportStateCreateInfo)
        .setPRasterizationState(&_rasterizationStateCreateInfo)
        .setPMultisampleState(&_multisampleStateCreateInfo)
        .setPDepthStencilState(&_depthStencilStateCreateInfo)
        .setPColorBlendState(&_colorBlendStateCreateInfo)
        .setPDynamicState(&dynamicStateCreateInfo)
        .setRenderPass(Graphics::RenderPass)
        .setLayout(Graphics::PipelineLayout);

    vk::Result result;
    std::tie(result, _pipeline) = Graphics::Device.createGraphicsPipeline(nullptr, pipelineCreateInfo);

    vk::detail::resultCheck(result, "vk::Device::createGraphicsPipeline",
        { vk::Result::eSuccess, vk::Result::ePipelineCompileRequired }
    );

    return true;
}

DUSK_API
void VulkanPipeline::Destroy()
{
    Graphics::Device.destroyPipeline(_pipeline);
    _pipeline = VK_NULL_HANDLE;
}

} // namespace dusk