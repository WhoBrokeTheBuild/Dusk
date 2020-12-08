#ifndef DUSK_VULKAN_PIPELINE_HPP
#define DUSK_VULKAN_PIPELINE_HPP

#include <Dusk/Vulkan/VulkanConfig.hpp>

#include <Dusk/Pipeline.hpp>

namespace Dusk {

class DUSK_VULKAN_API VulkanPipeline : public Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(VulkanPipeline)

    VulkanPipeline() = default;

    virtual ~VulkanPipeline() = default;

    inline std::string GetClassID() const override {
        return "Dusk::VulkanPipeline";
    }

    void Bind() override;

private:

    VkPolygonMode GetVkPolygonMode() const;

    VkCullModeFlags GetVkCullMode() const;

    VkFrontFace GetVkFrontFace() const;

    VkBlendFactor GetVkBlendFactor(BlendFactor factor) const;

    VkBlendOp GetVkBlendOp(BlendOperation op) const;

}; // class VulkanPipeline

} // namespace Dusk

#endif // DUSK_VULKAN_PIPELINE_HPP