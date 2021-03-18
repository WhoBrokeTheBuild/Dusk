#include <Dusk/Vulkan/VulkanShader.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Util.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Vulkan/VulkanGraphicsDriver.hpp>

#include <fstream>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
void VulkanShader::Terminate()
{
    auto gfx = VulkanGraphicsDriver::GetInstance();

    for (auto& shaderModule : _shaderModuleList) {
        vkDestroyShaderModule(gfx->GetDevice(), shaderModule, nullptr);
    }
    _shaderModuleList.clear();
}

DUSK_VULKAN_API
bool VulkanShader::LoadFromFiles(const std::vector<string>& filenameList, bool useAssetPath /*= true*/)
{
    DuskBenchmarkStart();

    for (const auto& filename : filenameList) {
        if (!LoadSPV(Path(filename), useAssetPath)) {
            if (!LoadSPV(Path(filename + ".spv"), useAssetPath)) {
                LogError(DUSK_ANCHOR, "Failed to load '{}'", filename);
                return false;
            }
        }
    }

    DuskBenchmarkEnd();
    return true;
}

DUSK_VULKAN_API
bool VulkanShader::LoadSPV(const Path& path, bool useAssetPath)
{
    auto gfx = VulkanGraphicsDriver::GetInstance();

    std::ifstream file;

    if (useAssetPath) {
        for (const auto& assetPath : GetAssetPathList()) {
            Path fullPath = assetPath / "Shaders" / path;
            LogVerbose(DUSK_ANCHOR, "Checking '{}'", fullPath);

            file.open(fullPath, std::ios::binary);
            if (file.is_open()) {
                break;
            }
        }
    }
    else {
        file.open(path, std::ios::binary);
    }

    if (!file.is_open()) {
        return false;
    }

    file.unsetf(std::ios::skipws);

    LogVerbose(DUSK_ANCHOR, "Loading SPIR-V shader '{}'", path);

    std::vector<uint8_t> data(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    VkShaderStageFlagBits type = GetVkShaderType(path);
    if (type == VK_SHADER_STAGE_ALL) {
        LogError(DUSK_ANCHOR, "Failed to determine shader type of '{}'", path);
        return false;
    }

    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = data.size(),
        .pCode = reinterpret_cast<const uint32_t *>(data.data()),
    };

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(gfx->GetDevice(), &shaderModuleCreateInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        LogError(DUSK_ANCHOR, "Failed to create shader module");
        return false;
    }

    VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = type,
        .module = shaderModule, // This conflicts with the `module` keyword in C++20
        .pName = "main", // TODO: Update
        .pSpecializationInfo = nullptr,
    };

    _shaderModuleList.push_back(shaderModule);
    _shaderStageList.push_back(shaderStageCreateInfo);

    return true;
}

DUSK_VULKAN_API
VkShaderStageFlagBits VulkanShader::GetVkShaderType(const Path& path)
{
    string filename = path.GetFilename();
    string ext = path.GetExtension();

    size_t pivot = filename.find_last_of('.');
    if (pivot == string::npos) {
        return VK_SHADER_STAGE_ALL; // Invalid
    }
    ext = GetExtension(filename.substr(0, pivot));

    if (ext == "vert" || ext == "vertex") {
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    else if (ext == "frag" || ext == "fragment") {
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    else if (ext == "tesc" || ext == "tesscontrol") {
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    }
    else if (ext == "tese" || ext == "tesseval") {
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    }
    else if (ext == "comp" || ext == "compute") {
        return VK_SHADER_STAGE_COMPUTE_BIT;
    }

    return VK_SHADER_STAGE_ALL; // Invalid
}

} // namespace Dusk::Vulkan