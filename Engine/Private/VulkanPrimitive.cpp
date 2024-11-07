#include <Dusk/VulkanPrimitive.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/ShaderTransform.hpp>
#include <Dusk/ShaderMaterial.hpp>
#include <Dusk/Array.hpp>
#include <Dusk/Log.hpp>

namespace dusk {

VulkanPrimitive::VulkanPrimitive()
{
    _material = Graphics::DefaultMaterial;
}

VulkanPrimitive::~VulkanPrimitive()
{
    Destroy();
}

bool VulkanPrimitive::Create(
    Span<PrimitiveVertex> vertexList,
    VkPrimitiveTopology topology /*= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST*/,
    bool haveTangents /*= true*/
) {
    Destroy();

    _count = vertexList.size();
    _topology = topology;

    calculateBounds(vertexList);

    if (not haveTangents) {
        Log(DUSK_ANCHOR, "Calculating Tangents");
        calculateTangents({}, vertexList);
    }

    _indexed = false;

    _vertexBuffer.Create(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        vertexList.size_bytes(),
        vertexList.data()
    );

    createDescriptorSet();
    
    return true;
}

bool VulkanPrimitive::Create(
    Span<uint32_t> indexList,
    Span<PrimitiveVertex> vertexList,
    VkPrimitiveTopology topology /*= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST*/,
    bool haveTangents /*= true*/
) {
    Destroy();

    _count = vertexList.size();
    _topology = topology;

    calculateBounds(vertexList);

    if (not haveTangents) {
        Log(DUSK_ANCHOR, "Calculating Tangents");
        calculateTangents(indexList, vertexList);
    }

    if (not indexList.empty()) {
        _indexed = true;
        _count = indexList.size();

        _indexBuffer.Create(
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VMA_MEMORY_USAGE_GPU_ONLY,
            indexList.size_bytes(),
            indexList.data()
        );
    }

    _vertexBuffer.Create(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        vertexList.size_bytes(),
        vertexList.data()
    );

    createDescriptorSet();

    return true;
}

void VulkanPrimitive::Destroy()
{
    _indexed = false;
    _count = 0;

    _indexBuffer.Destroy();
    _vertexBuffer.Destroy();

    // TODO: Free _descriptorSet?
    _descriptorSet = VK_NULL_HANDLE;
}

void VulkanPrimitive::UpdateDescriptorSet(VulkanBuffer * transformBuffer)
{
    auto globalsBufferInfo = VkDescriptorBufferInfo{
        .buffer = Graphics::GlobalsBuffer->GetVkBuffer(),
        .range = VK_WHOLE_SIZE,
    };

    auto transformBufferInfo = VkDescriptorBufferInfo{
        .buffer = transformBuffer->GetVkBuffer(),
        .range = VK_WHOLE_SIZE,
    };

    auto materialBufferInfo = VkDescriptorBufferInfo{
        .buffer = _material->GetBuffer()->GetVkBuffer(),
        .range = VK_WHOLE_SIZE,
    };

    auto baseColorMap = _material->GetBaseColorMap();
    auto baseColorMapImageInfo = VkDescriptorImageInfo{
        .sampler = baseColorMap->GetSampler(),
        .imageView = baseColorMap->GetImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    auto normalMap = _material->GetNormalMap();
    auto normalMapImageInfo = VkDescriptorImageInfo{
        .sampler = normalMap->GetSampler(),
        .imageView = normalMap->GetImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    auto metallicRoughnessMap = _material->GetMetallicRoughnessMap();
    auto metallicRoughnessMapImageInfo = VkDescriptorImageInfo{
        .sampler = metallicRoughnessMap->GetSampler(),
        .imageView = metallicRoughnessMap->GetImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    auto emissiveMap = _material->GetEmissiveMap();
    auto emissiveMapImageInfo = VkDescriptorImageInfo{
        .sampler = emissiveMap->GetSampler(),
        .imageView = emissiveMap->GetImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    auto occlusionMap = _material->GetOcclusionMap();
    auto occlusionMapImageInfo = VkDescriptorImageInfo{
        .sampler = occlusionMap->GetSampler(),
        .imageView = occlusionMap->GetImageView(),
        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    Array<VkWriteDescriptorSet, 8> writeDescriptorSetList = {
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderGlobals::Binding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &globalsBufferInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderTransform::Binding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &transformBufferInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderMaterial::Binding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &materialBufferInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderMaterial::BaseColorMapBinding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &baseColorMapImageInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderMaterial::NormalMapBinding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &normalMapImageInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderMaterial::MetallicRoughnessMapBinding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &metallicRoughnessMapImageInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderMaterial::EmissiveMapBinding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &emissiveMapImageInfo,
        },
        VkWriteDescriptorSet{
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = _descriptorSet,
            .dstBinding = ShaderMaterial::OcclusionMapBinding,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .pImageInfo = &occlusionMapImageInfo,
        },
    };

    vkUpdateDescriptorSets(
        Graphics::Device,
        writeDescriptorSetList.size(),
        writeDescriptorSetList.data(),
        0, nullptr
    );
}

void VulkanPrimitive::GenerateCommands(VkCommandBuffer commandBuffer)
{
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        Graphics::PipelineLayout,
        0, 1, &_descriptorSet,
        0, nullptr
    );

    if (_indexed) {
        vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
 
    // TODO: Enable
    // vkCmdSetPrimitiveTopology(commandBuffer, _topology);


    VkBuffer vertexBuffer = _vertexBuffer.GetVkBuffer();
    VkDeviceSize vertexOffset = 0;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &vertexOffset);

    if (_indexed) {
        vkCmdDrawIndexed(commandBuffer, _count, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(commandBuffer, _count, 1, 0, 0);
    }
}

void VulkanPrimitive::createDescriptorSet()
{
    VkResult result;

    auto descriptorSetAllocateInfo = VkDescriptorSetAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = Graphics::DescriptorPool,
        .descriptorSetCount = 1,
        .pSetLayouts = &Graphics::DescriptorSetLayout,
    };

    result = vkAllocateDescriptorSets(
        Graphics::Device,
        &descriptorSetAllocateInfo,
        &_descriptorSet
    );
    CheckVkResult("vkAllocateDescriptorSets", result);
}

void VulkanPrimitive::calculateBounds(Span<PrimitiveVertex> vertexList)
{
    for (const auto& vertex : vertexList) {
        _bounds.Extend(vertex.Position);
    }

    Log(DUSK_ANCHOR, "Lower: {}", glm::to_string(_bounds.Lower));
    Log(DUSK_ANCHOR, "Upper: {}", glm::to_string(_bounds.Upper));
}

void VulkanPrimitive::calculateTangents(Span<uint32_t> indexList, Span<PrimitiveVertex> vertexList)
{
    // Assumes Counter(?) Clockwise

    // Computing tangents for other topologies can cause issues with averaging and such
    // best to just let the modeling software generate those tangents
    if (_topology != VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST) {
        return;
    }

    auto processTriangle = [](PrimitiveVertex& v1, PrimitiveVertex& v2, PrimitiveVertex& v3)
    {
        vec4 v = v2.Position - v1.Position;
        vec4 w = v3.Position - v1.Position;
        vec2 s = v2.TexCoord - v1.TexCoord;
        vec2 t = v3.TexCoord - v1.TexCoord;

        float direction = 1.0f;
        if (((t.x * s.y) - (t.y * s.x)) < 0.0f) {
            direction = -1.0f;
        }

        vec4 tangent = {
            (w.x * s.y - v.x * t.y) * direction,
            (w.y * s.y - v.y * t.y) * direction,
            (w.z * s.y - v.z * t.y) * direction,
            1.0f,
        };

        v1.Tangent = glm::normalize(tangent - v1.Normal * tangent * v1.Normal);
        v2.Tangent = glm::normalize(tangent - v2.Normal * tangent * v2.Normal);
        v3.Tangent = glm::normalize(tangent - v3.Normal * tangent * v3.Normal);

        v1.Tangent.w = 1.0f;
        v2.Tangent.w = 1.0f;
        v3.Tangent.w = 1.0f;
    };

    if (not indexList.empty()) {
        for (size_t i = 0; i < indexList.size(); i += 3) {
            processTriangle(
                vertexList[indexList[i + 0]],
                vertexList[indexList[i + 1]],
                vertexList[indexList[i + 2]]
            );
        }
    }
    else {
        for (size_t i = 0; i < vertexList.size(); i += 3) {
            processTriangle(
                vertexList[i + 0],
                vertexList[i + 1],
                vertexList[i + 2]
            );
        }
    }
}

} // namespace dusk