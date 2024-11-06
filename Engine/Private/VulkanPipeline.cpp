#include <Dusk/VulkanPipeline.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/Array.hpp>
#include <Dusk/PrimitiveVertex.hpp>

namespace dusk {

DUSK_API
VulkanPipeline::VulkanPipeline()
{
    _inputAssemblyStateCreateInfo =  {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = false,
    };

    _rasterizationStateCreateInfo =  {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = false,
        .rasterizerDiscardEnable = false,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_NONE,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = false,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    _multisampleStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = Graphics::MSAASampleCount,
        .sampleShadingEnable = false,
    };

    _colorBlendAttachmentState =  {
        .blendEnable = false,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = (
              VK_COLOR_COMPONENT_R_BIT
            | VK_COLOR_COMPONENT_G_BIT
            | VK_COLOR_COMPONENT_B_BIT
            | VK_COLOR_COMPONENT_A_BIT
        ),
    };

    _colorBlendStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = false,
        .logicOp = VK_LOGIC_OP_COPY,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
    };

    _depthStencilStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable = true,
        .depthWriteEnable = true,
        .depthCompareOp = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = false,
        .stencilTestEnable = false,
    };
}

DUSK_API
VulkanPipeline::~VulkanPipeline()
{
    Destroy();
}

DUSK_API
bool VulkanPipeline::Create(VulkanShader::Pointer shader)
{
    VkResult result;

    Destroy();

    _shader = shader;

    auto stageList = _shader->GetPipelineShaderStageCreateInfoList();
    // auto pipelineLayout = _shader->GetPipelineLayout();

    Array<VkVertexInputBindingDescription, 1> vertexInputBindingDescriptionList = {
        VkVertexInputBindingDescription{
            .binding = 0,
            .stride = sizeof(PrimitiveVertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        },
    };

    Array<VkVertexInputAttributeDescription, 7> vertexInputAttributeDescriptionList = {
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::Position,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(PrimitiveVertex, Position),
        },
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::Normal,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(PrimitiveVertex, Normal),
        },
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::Tangent,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(PrimitiveVertex, Tangent),
        },
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::Color,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(PrimitiveVertex, Color),
        },
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::TexCoord,
            .binding = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .offset = offsetof(PrimitiveVertex, TexCoord),
        },
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::Joints,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_UINT,
            .offset = offsetof(PrimitiveVertex, Joints),
        },
        VkVertexInputAttributeDescription{
            .location = PrimitiveVertex::AttributeLocation::Weights,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32A32_SFLOAT,
            .offset = offsetof(PrimitiveVertex, Weights),
        },
    };

    auto viewportStateCreateInfo = VkPipelineViewportStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1,
    };

    Array<VkDynamicState, 2> dynamicStateList = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        // VkDynamicState::ePrimitiveTopology,
    };

    // TODO: eDepthBias, eStencilCompareMask, eStencilReference, eStencilWriteMask

    auto dynamicStateCreateInfo = VkPipelineDynamicStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = uint32_t(dynamicStateList.size()),
        .pDynamicStates = dynamicStateList.data(),
    };

    auto vertexInputStateCreateInfo = VkPipelineVertexInputStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = uint32_t(vertexInputBindingDescriptionList.size()),
        .pVertexBindingDescriptions = vertexInputBindingDescriptionList.data(),
        .vertexAttributeDescriptionCount = uint32_t(vertexInputAttributeDescriptionList.size()),
        .pVertexAttributeDescriptions = vertexInputAttributeDescriptionList.data(),
    };

    _colorBlendStateCreateInfo.attachmentCount = 1;
    _colorBlendStateCreateInfo.pAttachments = &_colorBlendAttachmentState;

    auto pipelineCreateInfo = VkGraphicsPipelineCreateInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = uint32_t(stageList.size()),
        .pStages = stageList.data(),
        .pVertexInputState = &vertexInputStateCreateInfo,
        .pInputAssemblyState = &_inputAssemblyStateCreateInfo,
        // .setPTessellationState()
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &_rasterizationStateCreateInfo,
        .pMultisampleState = &_multisampleStateCreateInfo,
        .pDepthStencilState = &_depthStencilStateCreateInfo,
        .pColorBlendState = &_colorBlendStateCreateInfo,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = Graphics::PipelineLayout,
        .renderPass = Graphics::RenderPass,
    };

    result = vkCreateGraphicsPipelines(
        Graphics::Device,
        nullptr,
        1, &pipelineCreateInfo,
        nullptr,
        &_pipeline
    );
    CheckVkResult("vkCreateGraphicsPipelines", result);

    return true;
}

DUSK_API
void VulkanPipeline::Destroy()
{
    vkDestroyPipeline(Graphics::Device, _pipeline, nullptr);
    _pipeline = VK_NULL_HANDLE;
}

} // namespace dusk