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

    virtual ~VulkanPipeline();

    inline std::string GetClassID() const override {
        return "Dusk::VulkanPipeline";
    }

    void Create() override;

    inline VkPipeline GetVkPipeline() const {
        return _vkPipeline;
    }

private:

    VkPolygonMode GetVkPolygonMode() const;

    VkCullModeFlags GetVkCullMode() const;

    VkFrontFace GetVkFrontFace() const;

    VkPrimitiveTopology GetVkPrimitiveTopology() const;

    VkBlendFactor GetVkBlendFactor(BlendFactor factor) const;

    VkBlendOp GetVkBlendOp(BlendOperation op) const;

    VkPipeline _vkPipeline;

}; // class VulkanPipeline

} // namespace Dusk::Vulkan

#endif // DUSK_VULKAN_PIPELINE_HPP