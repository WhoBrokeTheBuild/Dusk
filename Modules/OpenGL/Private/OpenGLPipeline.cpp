#include <Dusk/OpenGL/OpenGLPipeline.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/OpenGL/OpenGLShader.hpp>

#include <cassert>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLPipeline::Bind()
{
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    glm::ivec2 windowSize = gfx->GetWindowSize();

    OpenGLShader * shader = static_cast<OpenGLShader *>(_shader.get());
    OpenGLShader * debugShader = static_cast<OpenGLShader *>(gfx->GetActiveDebugShader());

    if (debugShader) {
        debugShader->Bind();
    }
    else if (shader) {
        shader->Bind();
    }

    glViewport(0, 0, windowSize.x, windowSize.y);

    glFrontFace(GetGLFrontFace(_frontFace));

    if (_cullMode == CullMode::None) {
        glDisable(GL_CULL_FACE);
    }
    else {
        glEnable(GL_CULL_FACE);
        glCullFace(GetGLCullMode(_cullMode));
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
        glDepthFunc(GetGLCompareOperation(_depthCompareOperation));
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

    return true;
}

} // namespace Dusk::OpenGL