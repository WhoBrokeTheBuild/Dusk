#include <Dusk/OpenGL/OpenGLPipeline.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/OpenGL/OpenGLShader.hpp>

#include <cassert>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLPipeline::Initialize()
{
    return true;
}

DUSK_OPENGL_API
void OpenGLPipeline::Terminate()
{
}

DUSK_OPENGL_API
bool OpenGLPipeline::Bind()
{
    // TODO: Fix
    float WIDTH = 1024.0f;
    float HEIGHT = 768.0f;

    OpenGLShader * shader = DUSK_OPENGL_SHADER(_shader.get());
    if (shader) {
        shader->Bind();
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glFrontFace(GetGLFrontFace(_frontFace));

    if (_cullMode == CullMode::None) {
        glDisable(GL_CULL_FACE);
    }
    else {
        glCullFace(GetGLCullMode(_cullMode));
    }

    switch (_cullMode) {
    case CullMode::None:
        glDisable(GL_CULL_FACE);
        break;
    case CullMode::Front:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
    case CullMode::Back:
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
    default: ;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(_fillMode));

    switch(_depthBiasMode) {
    case DepthBiasMode::Clamp:
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(_depthBiasSlopeFactor, _depthBiasFactor);
        break;
    case DepthBiasMode::Clip:
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDepthFunc(GL_LEQUAL);
        break;
    }

    if (_blendEnabled) {
        glEnable(GL_BLEND);

        glBlendFuncSeparate(
            GetGLBlendFactor(_srcColorFactor),
            GetGLBlendFactor(_dstColorFactor),
            GetGLBlendFactor(_srcAlphaFactor),
            GetGLBlendFactor(_dstAlphaFactor)
        );

        glBlendEquationSeparate(
            GetGLBlendOperation(_colorBlendOp),
            GetGLBlendOperation(_alphaBlendOp)
        );
    }
    else {
        glDisable(GL_BLEND);
    }

    // glDepthFunc(GetGLCompareOperation())

    return true;
}

} // namespace Dusk::OpenGL