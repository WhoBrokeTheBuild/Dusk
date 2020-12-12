#include <Dusk/Vulkan/VulkanPipeline.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>
#include <Dusk/Vulkan/VulkanShader.hpp>
#include <Dusk/Vulkan/VulkanMesh.hpp>

namespace Dusk::Vulkan {

VulkanPipeline::~VulkanPipeline()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());
 
    vkDestroyPipeline(gfx->GetDevice(), _vkPipeline, nullptr);
}

void VulkanPipeline::Create()
{
    VkResult vkResult;

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    VulkanShader * shader = DUSK_VULKAN_SHADER(_shader);
    if (!shader) {
        DuskLogError("Trying to bind a Vulkan VulkanPipeline with no shader");
        return;
    }

    VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh);
    if (!mesh) {
        DuskLogError("Trying to bind a Vulkan VulkanPipeline with no mesh");
        return;
    }

    const auto& stages = shader->GetStages();

    VkExtent2D extent = gfx->GetSwapChainExtent();

    VkViewport viewport = {
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    VkRect2D scissor = {
        .offset = { 0, 0 },
        .extent = { extent.width, extent.height },
    };

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = &viewport,
        .scissorCount = 1,
        .pScissors = &scissor,
    };

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = (_depthBiasMode == DepthBiasMode::Clamp),
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = GetVkPolygonMode(),
        .cullMode = GetVkCullMode(),
        .frontFace = GetVkFrontFace(),
        .depthBiasEnable = VK_TRUE,
        .depthBiasConstantFactor = _depthBiasFactor,
        .depthBiasClamp = _depthBiasClamp,
        .depthBiasSlopeFactor = _depthBiasSlopeFactor,
        .lineWidth = 1.0f,
    };

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
    };

    // VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
    //     .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
    //     .pNext = nullptr,
    //     .flags = 0,
    //     .depthTestEnable,
    //     .depthWriteEnable,
    //     .depthCompareOp,
    //     .depthBoundsTestEnable,
    //     .stencilTestEnable,
    //     .front,
    //     .back,
    //     .minDepthBounds,
    //     .maxDepthBounds,
    // };

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
        .blendEnable = _blendEnabled,
        .srcColorBlendFactor = GetVkBlendFactor(_srcColorFactor),
        .dstColorBlendFactor = GetVkBlendFactor(_dstColorFactor),
        .colorBlendOp = GetVkBlendOp(_colorBlendOp),
        .srcAlphaBlendFactor = GetVkBlendFactor(_srcAlphaFactor),
        .dstAlphaBlendFactor = GetVkBlendFactor(_dstAlphaFactor),
        .alphaBlendOp = GetVkBlendOp(_alphaBlendOp),
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                        | VK_COLOR_COMPONENT_G_BIT
                        | VK_COLOR_COMPONENT_B_BIT
                        | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachmentState,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
    };

    // TODO: Move

    // TransformData


    const auto& bindings = mesh->GetVkVertexInputBindings();
    const auto& attributes = mesh->GetVkVertexInputAttributes();

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size()),
        .pVertexBindingDescriptions = bindings.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size()),
        .pVertexAttributeDescriptions = attributes.data(),
    };

    VkPipelineInputAssemblyStateCreateInfo vertexInputAssemblyStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = GetVkPrimitiveTopology(),
        .primitiveRestartEnable = VK_FALSE,
    };

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stageCount = static_cast<uint32_t>(stages.size()),
        .pStages = stages.data(),
        .pVertexInputState = &vertexInputStateCreateInfo,
        .pInputAssemblyState = &vertexInputAssemblyStateCreateInfo,
        .pTessellationState = nullptr,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        // .pDepthStencilState = &depthStencilStateCreateInfo,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = nullptr,
        .layout = gfx->GetVkPipelineLayout(),
        .renderPass = gfx->GetRenderPass(),
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0,
    };

    vkResult = vkCreateGraphicsPipelines(gfx->GetDevice(), nullptr, 1, &pipelineCreateInfo, nullptr, &_vkPipeline);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("Failed to create graphics pipeline");
        return;
    }
}

VkPolygonMode VulkanPipeline::GetVkPolygonMode() const
{
    switch (_fillMode) {
    case FillMode::Line:
        return VK_POLYGON_MODE_LINE;
    case FillMode::Fill:
        return VK_POLYGON_MODE_FILL;
    }

    DuskLogFatal("Unexpected FillMode: %d", (int)_fillMode);
}

VkCullModeFlags VulkanPipeline::GetVkCullMode() const
{
    switch (_cullMode) {
    case CullMode::None:
        return 0;
    case CullMode::Front:
        return VK_CULL_MODE_FRONT_BIT;
    case CullMode::Back:
        return VK_CULL_MODE_BACK_BIT;
    }
    
    DuskLogFatal("Unexpected CullMode: %d", (int)_cullMode);
}

VkFrontFace VulkanPipeline::GetVkFrontFace() const
{
    switch (_frontFace) {
    case FrontFace::Clockwise:
        return VK_FRONT_FACE_CLOCKWISE;
    case FrontFace::CounterClockwise:
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }
    
    DuskLogFatal("Unexpected FrontFace: %d", (int)_frontFace);
}

VkPrimitiveTopology VulkanPipeline::GetVkPrimitiveTopology() const
{
    switch (_primitiveTopology) {
    case PrimitiveTopology::Points:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveTopology::Lines:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveTopology::LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveTopology::Triangles:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveTopology::TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }

    DuskLogFatal("Unexpected PrimitiveTopology: %d", (int)_primitiveTopology);
}

VkBlendFactor VulkanPipeline::GetVkBlendFactor(BlendFactor factor) const
{
    switch (factor) {
    case BlendFactor::Zero:
        return VK_BLEND_FACTOR_ZERO;
    case BlendFactor::One:
        return VK_BLEND_FACTOR_ONE;
    case BlendFactor::SrcColor:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case BlendFactor::SrcAlpha:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstColor:
        return VK_BLEND_FACTOR_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case BlendFactor::DstAlpha:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor:
        return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha:
        return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturated:
        return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    }
    
    DuskLogFatal("Unexpected BlendFactor: %d", (int)factor);
}

VkBlendOp VulkanPipeline::GetVkBlendOp(BlendOperation op) const
{
    switch (op) {
    case BlendOperation::Add:
        return VK_BLEND_OP_ADD;
    case BlendOperation::Subtract:
        return VK_BLEND_OP_SUBTRACT;
    case BlendOperation::ReverseSubtract:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    case BlendOperation::Min:
        return VK_BLEND_OP_MIN;
    case BlendOperation::Max:
        return VK_BLEND_OP_MAX;
    }
    
    DuskLogFatal("Unexpected BlendOperation: %d", (int)op);
}

} // namespace Dusk::Vulkan
