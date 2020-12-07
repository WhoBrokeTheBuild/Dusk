#ifndef DUSK_OPENGL_PIPELINE_HPP
#define DUSK_OPENGL_PIPELINE_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Pipeline.hpp>

namespace Dusk {

class DUSK_OPENGL_API OpenGLPipeline : public Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLPipeline)

    OpenGLPipeline() = default;

    virtual ~OpenGLPipeline() = default;

    inline std::string GetClassID() const override {
        return "Dusk::OpenGLPipeline";
    }

    void Bind() override;

private:

    GLenum GetGLBlendFactor(BlendFactor factor) const;

    GLenum GetGLBlendOperation(BlendOperation op) const;

}; // class OpenGLPipeline

} // namespace Dusk

#endif // DUSK_OPENGL_PIPELINE_HPP