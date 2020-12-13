#ifndef DUSK_VULKAN_PIPELINE_HPP
#define DUSK_VULKAN_PIPELINE_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Pipeline.hpp>

namespace Dusk::Vulkan {

#define DUSK_VULKAN_PIPELINE(x) (dynamic_cast<Dusk::Vulkan::VulkanPipeline *>(x))

class DUSK_VULKAN_API VulkanPipeline : public Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPipeline)

    VulkanPipeline() = default;

    virtual ~VulkanPipeline() = default;

    inline std::string GetClassID() const override {
        return "Dusk::VulkanPipeline";
    }

    bool Initialize() override;

    void Terminate() override;

    void GenerateBindCommands(VkCommandBuffer vkCommandBuffer);

    void GenerateDrawCommands(VkCommandBuffer vkCommandBuffer);

private:

    VkPolygonMode GetVkPolygonMode() const;

    VkCullModeFlags GetVkCullMode() const;

    VkFrontFace GetVkFrontFace() const;

    VkBlendFactor GetVkBlendFactor(BlendFactor factor) const;

    VkBlendOp GetVkBlendOp(BlendOperation op) const;

    VkPipeline _vkPipeline;

}; // class VulkanPipeline

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_PIPELINE_HPP