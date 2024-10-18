#include <Dusk/VulkanShader.hpp>
#include <Dusk/Exception.hpp>
#include <Dusk/Span.hpp>
#include <Dusk/Graphics.hpp>
#include <Dusk/Set.hpp>
#include <Dusk/File.hpp>
#include <Dusk/Log.hpp>

#include <fstream>

#include <spirv_reflect.h>

namespace dusk {

DUSK_API
VulkanShader::~VulkanShader()
{
    Destroy();
}

DUSK_API
bool VulkanShader::LoadFromFiles(const List<Path>& pathList)
{
    Destroy();

    auto assetPathList = GetAssetPathList();

    _pathList = {};
    for (const auto& path : pathList) {
        Log(DUSK_ANCHOR, "Loading {}", path.string());

        if (path.is_absolute()) {
            if (loadSPV(path)) {
                _pathList.push_back(path);
            }
        }
        else {
            for (const auto& assetPath : assetPathList) {
                auto fullPath = assetPath / path;
                if (loadSPV(fullPath)) {
                    _pathList.push_back(fullPath);
                    break;
                }
            }
        }
    }

    if (pathList.size() != _pathList.size()) {
        return false;
    }

    // TODO: setLoaded
    return true;
}

DUSK_API
void VulkanShader::Destroy()
{
    // Graphics::Device.destroyPipelineLayout(_pipelineLayout);
    // _pipelineLayout = VK_NULL_HANDLE;
    
    _pushConstantRangeList.clear();

    _pipelineShaderStageCreateInfoList.clear();

    // for (auto& descriptorSetLayout : _descriptorSetLayoutList) {
    //     Graphics::Device.destroyDescriptorSetLayout(descriptorSetLayout);
    // }
    // _descriptorSetLayoutList.clear();

    for (auto& shaderModule : _shaderModuleList) {
        Graphics::Device.destroyShaderModule(shaderModule);
    }
    _shaderModuleList.clear();

    for (auto& reflectShaderModule : _reflectShaderModuleList) {
        spvReflectDestroyShaderModule(&reflectShaderModule);
    }
    _reflectShaderModuleList.clear();
}

DUSK_API
bool VulkanShader::Reload()
{
    return LoadFromFiles(_pathList);
}

DUSK_API
bool VulkanShader::loadSPV(const Path& path)
{
    File file(path, File::Mode::Read);
    if (not file) {
        return false;
    }

    auto data = file.ReadAll<uint32_t>();
    if (not data) {
        Log(DUSK_ANCHOR, "Failed to read entire SPV file");
        return false;
    }

    size_t sizeInBytes = data->size() * sizeof(uint32_t);

    _reflectShaderModuleList.push_back({});
    auto& reflectShaderModule = _reflectShaderModuleList.back();

    auto result = spvReflectCreateShaderModule(sizeInBytes, data->data(), &reflectShaderModule);
    if (result != SPV_REFLECT_RESULT_SUCCESS) {
        Log(DUSK_ANCHOR, "spvReflectCreateShaderModule failed");
        return false;
    }

    auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo()
        .setCodeSize(sizeInBytes)
        .setPCode(data->data());

    _shaderModuleList.push_back(Graphics::Device.createShaderModule(shaderModuleCreateInfo));
    auto shaderModule = _shaderModuleList.back();

    _pipelineShaderStageCreateInfoList.push_back(
        vk::PipelineShaderStageCreateInfo()
            .setStage(vk::ShaderStageFlagBits(reflectShaderModule.shader_stage))
            .setModule(shaderModule)
            .setPName(reflectShaderModule.entry_point_name)
            .setPSpecializationInfo(VK_NULL_HANDLE)
    );

    for (uint32_t i = 0; i < reflectShaderModule.push_constant_block_count; ++i) {
        const auto& pushConstantBlock = reflectShaderModule.push_constant_blocks[i];

        _pushConstantRangeList.push_back(
            vk::PushConstantRange()
                .setOffset(pushConstantBlock.offset)
                .setSize(pushConstantBlock.size)
                .setStageFlags(vk::ShaderStageFlags(reflectShaderModule.shader_stage))
        );

        // std::println("Push Constant Range: {} {} {}",
        //     pushConstantBlock.offset,
        //     pushConstantBlock.size,
        //     vk::to_string(vk::ShaderStageFlags(reflectShaderModule.shader_stage))
        // );
    }

    // for (uint32_t i = 0; i < reflectShaderModule.input_variable_count; ++i) {
    //     auto& input = reflectShaderModule.input_variables[i];
    //     std::println("  Input {} Location {} ", input->name, input->location);
    // }

    // for (uint32_t i = 0; i < reflectShaderModule.output_variable_count; ++i) {
    //     auto& output = reflectShaderModule.output_variables[i];
    //     std::println("  Output {} Location {} ", output->name, output->location);
    // }

    Log(DUSK_ANCHOR, "Loaded {}", path.string());

    return true;
}

} // namespace dusk