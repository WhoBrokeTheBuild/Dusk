#include <Dusk/Model.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/Array.hpp>

#include "glTF.hpp"

namespace dusk {

DUSK_API
void Model::Create(List<Mesh::Pointer>&& meshList)
{
    VkResult result;
    
    _meshList = std::move(meshList);

    _shaderTransformBuffer.reset(new VulkanBuffer());
    _shaderTransformBuffer->Create(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        sizeof(ShaderTransform)
    );

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

    auto globalsBufferInfo = VkDescriptorBufferInfo{
        .buffer = Graphics::GlobalsBuffer->GetVkBuffer(),
        .range = VK_WHOLE_SIZE,
    };

    auto transformBufferInfo = VkDescriptorBufferInfo{
        .buffer = _shaderTransformBuffer->GetVkBuffer(),
        .range = VK_WHOLE_SIZE,
    };

    Array<VkWriteDescriptorSet, 2> writeDescriptorSetList = {
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
    };

    vkUpdateDescriptorSets(
        Graphics::Device,
        writeDescriptorSetList.size(),
        writeDescriptorSetList.data(),
        0, nullptr
    );
}

DUSK_API
bool Model::LoadFromFile(const Path& path)
{
    Free();

    Log(DUSK_ANCHOR, "Loading {}", path.string());

    auto assetPathList = GetAssetPathList();

    auto extension = path.extension();
    if (extension == ".glb" || extension == ".gltf") {
        glTF::Loader file;
        if (path.is_absolute()) {
            if (file.LoadFromFile(path)) {
                _path = path;
            }
        }
        else {
            for (const auto& assetPath : assetPathList) {
                auto fullPath = assetPath / path;
                Log(DUSK_ANCHOR, "Checking {}", fullPath.string());
                if (file.LoadFromFile(fullPath)) {
                    _path = path;
                    break;
                }
            }
        }

        if (_path.empty()) {
            return false;
        }

        Create(std::move(file.MeshList));
    }

    return true;
}

DUSK_API
void Model::Free()
{
    _meshList.clear();

    // TODO: Free _descriptorSet?
    _descriptorSet = VK_NULL_HANDLE;

    _shaderTransformBuffer.reset();
}

DUSK_API
void Model::Update(mat4 view, mat4 proj)
{
    if (not _shaderTransformBuffer) {
        return;
    }

    ShaderTransform shaderTransform;
    shaderTransform.Model = Transform.ToMatrix();
    shaderTransform.View = view;
    shaderTransform.Projection = proj;
    shaderTransform.UpdateMVP();

    _shaderTransformBuffer->WriteTo(0, sizeof(shaderTransform), &shaderTransform);
}

DUSK_API
void Model::GenerateCommands(VkCommandBuffer commandBuffer)
{
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        Graphics::PipelineLayout,
        0, 1, &_descriptorSet,
        0, nullptr
    );

    for (auto& mesh : _meshList) {
        mesh->GenerateCommands(commandBuffer);
    }
}

} // namespace dusk