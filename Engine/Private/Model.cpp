#include <Dusk/Model.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/Array.hpp>

#include "glTF.hpp"

namespace dusk {

DUSK_API
void Model::Create(List<Mesh::Pointer>&& meshList)
{
    _meshList = std::move(meshList);

    _shaderTransformBuffer.reset(new VulkanBuffer());
    _shaderTransformBuffer->Create(
        vk::BufferUsageFlagBits::eUniformBuffer,
        VMA_MEMORY_USAGE_CPU_TO_GPU,
        sizeof(ShaderTransform)
    );

    auto descriptorSetAllocateInfo = vk::DescriptorSetAllocateInfo()
        .setDescriptorPool(Graphics::DescriptorPool)
        .setDescriptorSetCount(1)
        .setSetLayouts({ Graphics::DescriptorSetLayout });

    auto descriptorSetList = Graphics::Device.allocateDescriptorSets(descriptorSetAllocateInfo);
    _descriptorSet = descriptorSetList.front();

    auto _globalsBufferInfo = vk::DescriptorBufferInfo()
        .setBuffer(Graphics::GlobalsBuffer->GetVkBuffer())
        .setRange(VK_WHOLE_SIZE);

    auto transformBufferInfo = vk::DescriptorBufferInfo()
        .setBuffer(_shaderTransformBuffer->GetVkBuffer())
        .setRange(VK_WHOLE_SIZE);

    Array<vk::WriteDescriptorSet, 2> writeDescriptorSetList = {
        vk::WriteDescriptorSet()
            .setDstSet(_descriptorSet)
            .setDstBinding(ShaderGlobals::Binding)
            .setBufferInfo({ _globalsBufferInfo })
            .setDescriptorType(vk::DescriptorType::eUniformBuffer),
        vk::WriteDescriptorSet()
            .setDstSet(_descriptorSet)
            .setDstBinding(ShaderTransform::Binding)
            .setBufferInfo({ transformBufferInfo })
            .setDescriptorType(vk::DescriptorType::eUniformBuffer),
    };

    Graphics::Device.updateDescriptorSets(writeDescriptorSetList, {});
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
void Model::GenerateCommands(vk::CommandBuffer commandBuffer)
{
    commandBuffer.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        Graphics::PipelineLayout,
        0, { _descriptorSet },
        {}
    );

    for (auto& mesh : _meshList) {
        mesh->GenerateCommands(commandBuffer);
    }
}

} // namespace dusk