#include <Dusk/Vulkan/Shader.hpp>
#include <Dusk/Vulkan/GraphicsDriver.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/Log.hpp>

#include <fstream>

namespace Dusk::Vulkan {

DUSK_VULKAN_API
bool Shader::LoadFromFiles(const std::vector<std::string>& filenames)
{
    DuskBenchmarkStart();

    GraphicsDriver * gfx = (GraphicsDriver *)GetGraphicsDriver();
    VkDevice dev = gfx->GetDevice();

    const auto& assetPaths = GetAssetPaths();

    std::ifstream file;

    for (const auto& filename : filenames) {
        for (const auto& path : assetPaths) {
            DuskLogVerbose("Checking '%s'", path + filename);

            file.open(path + filename, std::ios::binary);
            if (file.is_open()) {
                break;
            }
        }

        if (!file.is_open()) {
            DuskLogError("Failed to load '%s'", filename);
            return false;
        }

        file.unsetf(std::ios::skipws);

        DuskLogLoad("Loading SPIR-V shader '%s'", filename);

        std::vector<uint8_t> data(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        VkShaderStageFlagBits type = GetVkShaderType(filename);
        if (type == VK_SHADER_STAGE_ALL) {
            DuskLogError("Failed to determine shader type of '%s'", filename);
            return false;
        }

        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.codeSize = data.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(dev, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            DuskLogError("Failed to create shader module");
            return false;
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo;
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.pNext = nullptr;
        shaderStageInfo.flags = 0;
        shaderStageInfo.stage = type;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";
        shaderStageInfo.pSpecializationInfo = nullptr;

        _shaderStages.push_back(shaderStageInfo);
    }

    DuskBenchmarkEnd("Vulkan::Shader::LoadFromFiles");
    return true;
}

DUSK_VULKAN_API
void Shader::Bind()
{

}

DUSK_VULKAN_API
VkShaderStageFlagBits Shader::GetVkShaderType(const std::string& filename)
{
    std::string ext = GetExtension(filename);
    size_t pivot = filename.find_last_of('.');
    if (pivot == std::string::npos) {
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