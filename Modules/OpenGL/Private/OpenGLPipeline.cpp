#include <Dusk/OpenGL/OpenGLPipeline.hpp>

#include <Dusk/Benchmark.hpp>
#include <Dusk/OpenGL/OpenGLShader.hpp>

#include <cassert>

namespace Dusk {

DUSK_OPENGL_API
void OpenGLPipeline::Bind()
{
    // TODO: Fix
    float WIDTH = 1024.0f;
    float HEIGHT = 768.0f;

    OpenGLShader * shader = DUSK_OPENGL_SHADER(_shader);
    if (shader) {
        shader->Bind();
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    switch (_frontFace) {
    case FrontFace::Clockwise:
        glFrontFace(GL_CW);
        break;
    case FrontFace::CounterClockwise:
        glFrontFace(GL_CCW);
        break;
    default: ;
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

    switch (_fillMode) {
    case FillMode::Fill:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case FillMode::Line:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    default: ;
    }

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
}

GLenum OpenGLPipeline::GetGLBlendFactor(BlendFactor factor) const
{
    switch (factor) {
    case BlendFactor::Zero:
        return GL_ZERO;
    case BlendFactor::One:
        return GL_ONE;
    case BlendFactor::SrcColor:
        return GL_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case BlendFactor::SrcAlpha:
        return GL_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstColor:
        return GL_DST_COLOR;
    case BlendFactor::OneMinusDstColor:
        return GL_ONE_MINUS_DST_COLOR;
    case BlendFactor::DstAlpha:
        return GL_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor:
        return GL_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturated:
        return GL_SRC_ALPHA_SATURATE;
    }

    assert(false);
}

GLenum OpenGLPipeline::GetGLBlendOperation(BlendOperation op) const
{
    switch (op) {
    case BlendOperation::Add:
        return GL_FUNC_ADD;
    case BlendOperation::Subtract:
        return GL_FUNC_SUBTRACT;
    case BlendOperation::ReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
    case BlendOperation::Min:
        return GL_MIN;
    case BlendOperation::Max:
        return GL_MAX;
    }

    assert(false);
}

} // namespace Dusk