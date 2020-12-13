#include <Dusk/Vulkan/VulkanPipeline.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>
#include <Dusk/Vulkan/VulkanShader.hpp>
#include <Dusk/Vulkan/VulkanMesh.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
void VulkanPipeline::Terminate()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());
 
    vkDestroyPipeline(gfx->GetVkDevice(), _vkPipeline, nullptr);
}

DUSK_VULKAN_API
bool VulkanPipeline::Initialize()
{
    VkResult vkResult;

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    VulkanShader * shader = DUSK_VULKAN_SHADER(_shader.get());
    if (!shader) {
        DuskLogError("Trying to bind a Vulkan VulkanPipeline with no shader");
        return false;
    }

    VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh.get());
    if (!mesh) {
        DuskLogError("Trying to bind a Vulkan VulkanPipeline with no mesh");
        return false;
    }

    const auto& stages = shader->GetStages();

    VkExtent2D extent = gfx->GetVkSwapChainExtent();

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

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthTestEnable = (_depthCompareOperation != CompareOperation::Always),
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = GetVkCompareOp(_depthCompareOperation),
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        // .front,
        // .back,
        // .minDepthBounds,
        // .maxDepthBounds,
    };

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
        .topology = mesh->GetVkPrimitiveTopology(),
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
        .pDepthStencilState = &depthStencilStateCreateInfo,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = nullptr,
        .layout = gfx->GetVkPipelineLayout(),
        .renderPass = gfx->GetVkRenderPass(),
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0,
    };

    vkResult = vkCreateGraphicsPipelines(gfx->GetVkDevice(), nullptr, 1, &pipelineCreateInfo, nullptr, &_vkPipeline);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("Failed to create graphics pipeline");
        return false;
    }

    return true;
}

DUSK_VULKAN_API
void VulkanPipeline::GenerateBindCommands(VkCommandBuffer vkCommandBuffer)
{
    vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipeline);

    // TODO: Move to local variable post-conversion
    VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh.get());
    if (!mesh) {
        return;
    }

    mesh->GenerateBindCommands(vkCommandBuffer);
}

DUSK_VULKAN_API
void VulkanPipeline::GenerateDrawCommands(VkCommandBuffer vkCommandBuffer)
{
    // TODO: Move to local variable post-conversion
    VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh.get());
    if (!mesh) {
        return;
    }

    mesh->GenerateDrawCommands(vkCommandBuffer);
}

DUSK_VULKAN_API
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

DUSK_VULKAN_API
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

DUSK_VULKAN_API
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

DUSK_VULKAN_API
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

DUSK_VULKAN_API
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

VkCompareOp VulkanPipeline::GetVkCompareOp(CompareOperation op) const
{
    switch (op) {
    case CompareOperation::Never:
        return VK_COMPARE_OP_NEVER;
    case CompareOperation::Less:
        return VK_COMPARE_OP_LESS;
    case CompareOperation::Equal:
        return VK_COMPARE_OP_EQUAL;
    case CompareOperation::LessOrEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case CompareOperation::Greater:
        return VK_COMPARE_OP_GREATER;
    case CompareOperation::NotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case CompareOperation::GreaterOrEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case CompareOperation::Always:
        return VK_COMPARE_OP_ALWAYS;
    }

    DuskLogFatal("Unexpected CompareOperation: %d", (int)op);
}

} // namespace Dusk::Vulkan
