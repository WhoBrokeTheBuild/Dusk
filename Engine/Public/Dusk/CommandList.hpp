#ifndef DUSK_COMMAND_LIST_HPP
#define DUSK_COMMAND_LIST_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>

#include <Dusk/Pipeline.hpp>
#include <Dusk/Primitive.hpp>

namespace Dusk {

class Buffer;

enum class CommandType
{
    SetPipeline,
    SetIndexBuffer,
    SetVertexBuffers,
};

struct Viewport
{
    union
    {
        glm::vec2 Position;

        struct
        {
            float X;
            float Y;
            
        };
    };

    union 
    {
        glm::vec2 Size;

        struct
        {
            float Width;
            float Height;

        };
    };

    union 
    {
        glm::vec2 Depth;

        struct
        {
            float Near;
            float Far;

        };
    };
};

struct Command
{
    CommandType Type;

    union
    {
        struct
        {
            std::shared_ptr<Pipeline> Pipeline;

        } SetPipeline;

        struct
        {
            std::shared_ptr<Buffer> IndexBuffer;

        } SetIndexBuffer;

        struct
        {
            std::vector<std::shared_ptr<Buffer>> VertexBuffers;

        } SetVertexBuffers;

        struct
        {
            glm::vec2 Position;
            glm::vec2 Size;

        } SetScissor;

        struct 
        {
            Viewport Viewport;

        } SetViewport;

        struct
        {
            uint32_t VertexCount;
            uint32_t FirstVertex;
            

        } Draw;

        struct
        {
            uint32_t IndexCount;
            uint32_t FirstIndex;
            

        } DrawIndexed;

        struct 
        {
            std::shared_ptr<Buffer> Source;
            std::shared_ptr<Buffer> Destination;
            // Regions?

            
        } CopyBuffer;

    };
};

class Command
{
public:

    virtual void AddCommand(CommandList list) { };

    virtual void Execute() { };
};

class SetPipelineCommand : public Command
{
public:

    SetPipelineCommand(std::shared_ptr<Pipeline> pipeline)
        : _pipeline(pipeline)
    { }

private:

    std::shared_ptr<Pipeline> _pipeline;
};

class VulkanSetPipelineCommand : SetPipelineCommand
{
public:
    
    void AddCommand(CommandList list) override {
        auto vulkanCommandBuffer = DUSK_VULKAN_COMMAND_LIST(list.get());
        auto vulkanPipeline = DUSK_VULKAN_PIPELINE(_pipeline.get());
        vkCmdBindPipeline(vulkanCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);
    }
};

class OpenGLSetPipelineCommand : SetPipelineCommand
{
public:
    
    void Execute() override {
        auto openglPipeline = DUSK_OPENGL_PIPELINE(_pipeline.get());
        openglPipeline->Bind();
    }
};


// Command Bundles?

class DUSK_ENGINE_API CommandList : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(CommandList)

    CommandList() = default;

    virtual ~CommandList() = default;

    void Begin();
    void End();

    void AddSetPipelineCommand(std::shared_ptr<Pipeline> pipeline);

    void AddSetIndexBufferCommand(std::shared_ptr<Buffer> indexBuffer);
    void AddSetVertexBuffersCommand(std::vector<std::shared_ptr<Buffer>> vertexBuffers);

    void AddSetViewportCommand(Viewport viewport);

private:

    std::vector<Command> _commands;

}; // class CommandList

} // namespace Dusk

#endif // DUSK_COMMAND_LIST_HPP