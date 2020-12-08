#include <Dusk/Vulkan/VulkanPipeline.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>
#include <Dusk/Vulkan/VulkanShader.hpp>

namespace Dusk {

void VulkanPipeline::Bind()
{
    VkResult result;

    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    // TODO: Fix
    // swapChainExtent ?
    unsigned WIDTH = 1024;
    unsigned HEIGHT = 768;

    VulkanShader * shader = DUSK_VULKAN_SHADER(_shader);
    if (!shader) {
        DuskLogError("Trying to bind a Vulkan VulkanPipeline with no shader");
        return;
    }

    const auto& stages = shader->GetStages();



    VkAttachmentDescription colorAttachmentDescription = {
        .format = VK_FORMAT_R8G8B8A8_UINT,// ?
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        // TODO: Fix when we enable stencil buffer
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, // TODO: Investigate
    };

    VkAttachmentReference colorAttachmentReference = {
        .attachment = 0,
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpassDescription = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentReference,
    };

    VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = 1,
        .pAttachments = &colorAttachmentDescription,
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
    };

    // TODO: Move
    VkRenderPass renderPass;

    result = vkCreateRenderPass(gfx->GetDevice(), &renderPassCreateInfo, nullptr, &renderPass);
    if (result != VK_SUCCESS) {
        DuskLogError("Failed to create render pass");
        return;
    }



    VkViewport viewport = {
        .width = static_cast<float>(WIDTH),
        .height = static_cast<float>(HEIGHT),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    VkRect2D scissor = {
        .offset = { 0, 0 },
        .extent = { WIDTH, HEIGHT },
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
    VkPipelineLayout layout;

    VkPipelineLayoutCreateInfo layoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    result = vkCreatePipelineLayout(gfx->GetDevice(), &layoutCreateInfo, nullptr, &layout);
    if (result != VK_SUCCESS) {
        DuskLogError("Failed to create pipeline layout");
        return;
    }

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stageCount = static_cast<uint32_t>(stages.size()),
        .pStages = stages.data(),
        .pVertexInputState = nullptr, // TODO
        .pInputAssemblyState = nullptr, // TODO
        .pTessellationState = nullptr,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        // .pDepthStencilState = &depthStencilStateCreateInfo,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = nullptr,
        .layout = layout,
        .renderPass = renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0,
    };

    // TODO: Move
    VkPipeline pipeline;

    result = vkCreateGraphicsPipelines(gfx->GetDevice(), nullptr, 1, &pipelineCreateInfo, nullptr, &pipeline);
    if (result != VK_SUCCESS) {
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

} // namespace Dusk