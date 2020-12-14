#ifndef DUSK_OPENGL_PIPELINE_HPP
#define DUSK_OPENGL_PIPELINE_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Pipeline.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API OpenGLPipeline : public Pipeline
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLPipeline)

    OpenGLPipeline() = default;

    virtual ~OpenGLPipeline() = default;

    bool Initialize() override;

    void Terminate() override;

    bool Bind();

private:

    GLenum GetGLBlendFactor(BlendFactor factor) const;

    GLenum GetGLBlendOperation(BlendOperation op) const;

}; // class OpenGLPipeline

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_PIPELINE_HPP