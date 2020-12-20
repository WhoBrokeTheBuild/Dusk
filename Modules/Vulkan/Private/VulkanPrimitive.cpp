#include <Dusk/Vulkan/VulkanPrimitive.hpp>

#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
VulkanPrimitive::~VulkanPrimitive()
{
    VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

    // for (VkBuffer buffer : _vkBuffers) {
    //     vkDestroyBuffer(gfx->GetDevice(), buffer, nullptr);
    // }

    for (VkDeviceMemory memory : _vkBufferMemories) {
        vkFreeMemory(gfx->GetDevice(), memory, nullptr);
    }
}

bool VulkanPrimitive::Load(const std::unique_ptr<PrimitiveData>& data)
{
    bool result;


    // const auto& indexList = data->GetIndexList();
    // const auto& vertexList = data->GetVertexList();

    // std::vector<Vertex> vertexList(positionList.size() / 4);

    // for (size_t i = 0; i < vertexList.size(); ++i) {
    //     vertexList[i].Position = {
    //         positionList[(i * 4) + 0],
    //         positionList[(i * 4) + 1],
    //         positionList[(i * 4) + 2],
    //         positionList[(i * 4) + 3],
    //     };

    //     if (!normalList.empty()) {
    //         vertexList[i].Normal = {
    //             normalList[(i * 4) + 0],
    //             normalList[(i * 4) + 1],
    //             normalList[(i * 4) + 2],
    //             normalList[(i * 4) + 3],
    //         };  
    //     }

    //     if (!tangentList.empty()) {
    //         vertexList[i].Tangent = {
    //             tangentList[(i * 4) + 0],
    //             tangentList[(i * 4) + 1],
    //             tangentList[(i * 4) + 2],
    //             tangentList[(i * 4) + 3],
    //         };  
    //     }

    //     if (!bitangentList.empty()) {
    //         vertexList[i].Bitangent = {
    //             bitangentList[(i * 4) + 0],
    //             bitangentList[(i * 4) + 1],
    //             bitangentList[(i * 4) + 2],
    //             bitangentList[(i * 4) + 3],
    //         };  
    //     }

    //     if (!colorList.empty()) {
    //         vertexList[i].Color = {
    //             colorList[(i * 4) + 0],
    //             colorList[(i * 4) + 1],
    //             colorList[(i * 4) + 2],
    //             colorList[(i * 4) + 3],
    //         };  
    //     }

    //     if (!uv1List.empty()) {
    //         vertexList[i].UV1 = {
    //             uv1List[(i * 2) + 0],
    //             uv1List[(i * 2) + 1],
    //         };  
    //     }

    //     if (!uv2List.empty()) {
    //         vertexList[i].UV2 = {
    //             uv1List[(i * 2) + 0],
    //             uv1List[(i * 2) + 1],
    //         };  
    //     }
    // }

    // result = AddBuffer(static_cast<const void*>(vertexList.data()), 
    //     vertexList.size(), sizeof(float), VertexAttribute::Position);

    // if (!result) {
    //     DuskLogError("Failed to create vertex buffer");
    //     return false;
    // }

    // if (indexList.empty()) {
    //     _indexed = false;
    //     _count = vertexList.size();
    // }
    // else {
    //     DuskLogFatal("UNTESTED");

    //     _indexed = true;
    //     _count = indexList.size();

    //     result = AddBuffer(static_cast<const void*>(indexList.data()), 
    //         indexList.size(), sizeof(uint32_t), VertexAttribute::Index);

    //     if (!result) {
    //         DuskLogError("Failed to create index buffer");
    //         return false;
    //     }
    // }

    return true;
}

// void VulkanPrimitive::GenerateCommands(VkCommandBuffer vkCommandBuffer)
// {
//     if (_indexed) {
//         vkCmdBindIndexBuffer(vkCommandBuffer, _vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
//     }

//     VkDeviceSize offsets[] = { 0 };
//     VkBuffer vertexBuffers[] = { _vkVertexBuffer };
//     vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vertexBuffers, offsets);
    
//     if (_indexed) {
//         vkCmdDrawIndexed(vkCommandBuffer, _count, 1, 0, 0, 0);
//     }
//     else {
//         vkCmdDraw(vkCommandBuffer, _count, 1, 0, 0);
//     }
// }

// bool VulkanPrimitive::AddBuffer(const void * data, VkDeviceSize size, uint32_t stride, VkFormat format,
// VkVertexInputRate inputRate, VkBufferUsageFlags usage, VertexAttribute attribute)
// {
//     VulkanGraphicsDriver * gfx = DUSK_VULKAN_GRAPHICS_DRIVER(GetGraphicsDriver());

//     // uint32_t stride = elementSize;
//     // VkFormat format = VK_FORMAT_UNDEFINED;
//     // VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//     // VkBufferUsageFlags usage;

//     // switch (attribute) {
//     // case VertexAttribute::Index:
//     //     format = VK_FORMAT_R32_UINT;
//     //     usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
//     //     break;
//     // case VertexAttribute::Position:
//     //     stride = elementSize * 4;
//     //     format = VK_FORMAT_R32G32B32A32_SFLOAT;
//     //     usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//     // }

//     uint32_t binding = static_cast<uint32_t>(_vkBindings.size());

//     _vkBindings.push_back({
//         .binding = binding,
//         .stride = stride,
//         .inputRate = inputRate,
//     });

//     // if (attribute != VertexAttribute::Index) {
//     //     uint32_t location = GetVertexAttributeLocation(attribute);

//     //     _vkAttributes.push_back({
//     //         .location = location,
//     //         .binding = binding,
//     //         .format = format,
//     //         .offset = 0,
//     //     });
//     // }

//     VkBuffer buffer;
//     VkDeviceMemory memory;

//     if (!gfx->CreateBuffer(buffer, memory, size, usage, 
//         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
//         return false;
//     }

//     void * ptr;
//     vkMapMemory(gfx->GetDevice(), memory, 0, size, 0, &ptr);
//     memcpy(ptr, data, static_cast<size_t>(size));
//     vkUnmapMemory(gfx->GetDevice(), memory);

//     // _vkBuffers.push_back(buffer);
//     // _vkBufferMemories.push_back(memory);

//     return true;
// }

} // namespace Dusk::Vulkan
