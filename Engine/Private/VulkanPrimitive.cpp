#include <Dusk/VulkanPrimitive.hpp>

namespace dusk {

VulkanPrimitive::~VulkanPrimitive()
{
    Destroy();
}

bool VulkanPrimitive::Create(
    Span<PrimitiveVertex> vertexList,
    VkPrimitiveTopology topology /*= VkPrimitiveTopology::eTriangleList*/,
    bool haveTangents /*= true*/
) {
    Destroy();

    _count = vertexList.size();
    _topology = topology;

    if (not haveTangents) {
        calculateTangents({}, vertexList);
    }

    _indexed = false;

    _vertexBuffer.Create(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VMA_MEMORY_USAGE_GPU_ONLY,
        vertexList.size_bytes(),
        vertexList.data()
    );
    
    return true;
}

bool VulkanPrimitive::Create(
    Span<uint32_t> indexList,
    Span<PrimitiveVertex> vertexList,
    VkPrimitiveTopology topology /*= VkPrimitiveTopology::eTriangleList*/,
    bool haveTangents /*= true*/
) {
    Destroy();

    _count = vertexList.size();
    _topology = topology;

    if (not haveTangents) {
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

    return true;
}

void VulkanPrimitive::Destroy()
{
    _indexed = false;
    _count = 0;

    _indexBuffer.Destroy();
    _vertexBuffer.Destroy();
}

void VulkanPrimitive::GenerateCommands(VkCommandBuffer commandBuffer)
{
    if (_indexed) {
        vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
 
    // TODO: Enable
    // commandBuffer.setPrimitiveTopology(_topology);
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

void VulkanPrimitive::calculateTangents(Span<uint32_t> indexList, Span<PrimitiveVertex> vertexList)
{
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