#ifndef DUSK_COMMAND_LIST_HPP
#define DUSK_COMMAND_LIST_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>

#include <Dusk/Pipeline.hpp>
#include <Dusk/Primitive.hpp>

namespace Dusk {

struct CommandPipeline
{

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

    void BindPipeline();

    void BindIndexBuffer();
    void BindVertexBuffers();

    void CopyBuffer();

    void SetViewport();

private:

}; // class CommandList

} // namespace Dusk

#endif // DUSK_COMMAND_LIST_HPP