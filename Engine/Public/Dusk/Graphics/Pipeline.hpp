#ifndef DUSK_GRAPHICS_PIPELINE_HPP
#define DUSK_GRAPHICS_PIPELINE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Graphics/Shader.hpp>

#include <memory>

namespace Dusk {

class DUSK_ENGINE_API Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(Pipeline)

    Pipeline() = default;

    virtual ~Pipeline() = delete;

    void SetShader(std::shared_ptr<Shader> shader) = 0;

}; // class Pipeline

} // namespace Dusk

#endif // DUSK_GRAPHICS_PIPELINE_HPP