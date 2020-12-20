#include <Dusk/Vulkan/VulkanPipeline.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vertex.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>
#include <Dusk/Vulkan/VulkanShader.hpp>
#include <Dusk/Vulkan/VulkanMesh.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
void VulkanPipeline::Terminate()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());
 
    vkDestroyPipeline(gfx->GetDevice(), _vkPipeline, nullptr);
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

    auto polygonMode = GetVkPolygonMode(_fillMode);
    auto cullMode = GetVkCullMode(_cullMode);
    auto frontFace = GetVkFrontFace(_frontFace);

    if (!polygonMode || !cullMode || !frontFace) {
        return false;
    }

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = (_depthBiasMode == DepthBiasMode::Clamp),
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = polygonMode.value(),
        .cullMode = cullMode.value(),
        .frontFace = frontFace.value(),
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

    auto depthCompareOp = GetVkCompareOp(_depthCompareOperation);

    if (!depthCompareOp) {
        return false;
    }

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthTestEnable = (_depthCompareOperation != CompareOperation::Always),
        .depthWriteEnable = VK_TRUE,
        .depthCompareOp = depthCompareOp.value(),
        .depthBoundsTestEnable = VK_FALSE,
        .stencilTestEnable = VK_FALSE,
        // .front,
        // .back,
        // .minDepthBounds,
        // .maxDepthBounds,
    };

    auto srcColorFactor = GetVkBlendFactor(_srcColorFactor);
    auto dstColorFactor = GetVkBlendFactor(_dstColorFactor);
    auto colorBlendOp = GetVkBlendOp(_colorBlendOp);

    if (!srcColorFactor || !dstColorFactor || !colorBlendOp) {
        return false;
    }
    
    auto srcAlphaFactor = GetVkBlendFactor(_srcAlphaFactor);
    auto dstAlphaFactor = GetVkBlendFactor(_dstAlphaFactor);
    auto alphaBlendOp = GetVkBlendOp(_alphaBlendOp);

    if (!srcAlphaFactor || !dstAlphaFactor || !alphaBlendOp) {
        return false;
    }

    VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
        .blendEnable = _blendEnabled,
        .srcColorBlendFactor = srcColorFactor.value(),
        .dstColorBlendFactor = dstColorFactor.value(),
        .colorBlendOp = colorBlendOp.value(),
        .srcAlphaBlendFactor = srcAlphaFactor.value(),
        .dstAlphaBlendFactor = dstAlphaFactor.value(),
        .alphaBlendOp = alphaBlendOp.value(),
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

    std::array<VkVertexInputBindingDescription, 1> bindings = {
        VkVertexInputBindingDescription {
            .binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        }
    };

    std::array<VkVertexInputAttributeDescription, 9> attributes = {
        VkVertexInputAttributeDescription {
            .location = GetVertexAttributeLocation(VertexAttribute::Position),
        },
    };

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size()),
        .pVertexBindingDescriptions = bindings.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size()),
        .pVertexAttributeDescriptions = attributes.data(),
    };

    auto primitiveTopology = GetVkPrimitiveTopology(_primitiveTopology);

    VkPipelineInputAssemblyStateCreateInfo vertexInputAssemblyStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = primitiveTopology.value(),
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
        .layout = gfx->GetPipelineLayout(),
        .renderPass = gfx->GetRenderPass(),
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = 0,
    };

    vkResult = vkCreateGraphicsPipelines(gfx->GetDevice(), nullptr, 1, &pipelineCreateInfo, nullptr, &_vkPipeline);
    if (vkResult != VK_SUCCESS) {
        DuskLogError("Failed to create graphics pipeline");
        return false;
    }

    return true;
}

// DUSK_VULKAN_API
// void VulkanPipeline::GenerateBindCommands(VkCommandBuffer vkCommandBuffer)
// {
//     vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _vkPipeline);

//     // TODO: Move to local variable post-conversion
//     VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh.get());
//     if (!mesh) {
//         return;
//     }

//     mesh->GenerateBindCommands(vkCommandBuffer);
// }

// DUSK_VULKAN_API
// void VulkanPipeline::GenerateDrawCommands(VkCommandBuffer vkCommandBuffer)
// {
//     // TODO: Move to local variable post-conversion
//     VulkanMesh * mesh = DUSK_VULKAN_MESH(_mesh.get());
//     if (!mesh) {
//         return;
//     }

//     mesh->GenerateDrawCommands(vkCommandBuffer);
// }

} // namespace Dusk::Vulkan
