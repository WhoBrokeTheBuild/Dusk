#ifndef DUSK_PIPELINE_HPP
#define DUSK_PIPELINE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>

#include <Dusk/Shader.hpp>

namespace Dusk {

enum class FrontFace {
    Clockwise,
    CounterClockwise,
};

enum class CullMode {
    None,
    Front,
    Back,
};

enum class FillMode {
    Fill,
    Line,
};

enum class DepthBiasMode {
    Clamp,
    Clip,
};

enum class StepRate {
    Vertex,
    Instance,
};

enum class PrimitiveTopology {
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
};

enum class BlendOperation {
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max,
};

enum class BlendFactor {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstColor,
    OneMinusDstColor,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
    SrcAlphaSaturated, // TODO: Investigate
};

class DUSK_ENGINE_API Pipeline : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Pipeline)

    Pipeline() = default;

    virtual ~Pipeline() = default;

    inline std::string GetClassID() const override {
        return "Dusk::Pipeline";
    }

    inline void SetShader(Shader * shader) {
        _shader = shader;
    }

    inline void SetFrontFace(FrontFace frontFace) {
        _frontFace = frontFace;
    }

    inline void SetCullMode(CullMode mode) {
        _cullMode = mode;
    }

    inline void SetFillMode(FillMode mode) {
        _fillMode = mode;
    }

    inline void SetDepthBiasFactor(float factor) {
        _depthBiasFactor = factor;
    }

    inline void SetDepthBiasSlopeFactor(float factor) {
        _depthBiasSlopeFactor = factor;
    }

    inline void SetDepthBiasClamp(float clamp) {
        _depthBiasClamp = clamp;
    }

    inline void SetDepthBiasMode(DepthBiasMode mode) {
        _depthBiasMode = mode;
    }

    inline void SetPrimitiveTopology(PrimitiveTopology topology) {
        _primitiveTopology = topology;
    }

    virtual void Bind() = 0;

protected:

    Shader * _shader = nullptr;

    FrontFace _frontFace = FrontFace::CounterClockwise;

    CullMode _cullMode = CullMode::None;

    FillMode _fillMode = FillMode::Fill;

    float _depthBiasFactor = 0.0f;

    float _depthBiasSlopeFactor = 0.0f;

    float _depthBiasClamp = 0.0f;

    DepthBiasMode _depthBiasMode = DepthBiasMode::Clamp;

    PrimitiveTopology _primitiveTopology = PrimitiveTopology::Triangles;


    bool _blendEnabled = false;

    BlendOperation _colorBlendOp = BlendOperation::Add;

    BlendOperation _alphaBlendOp = BlendOperation::Add;

    BlendFactor _srcColorFactor = BlendFactor::One;

    BlendFactor _dstColorFactor = BlendFactor::One;

    BlendFactor _srcAlphaFactor = BlendFactor::One;

    BlendFactor _dstAlphaFactor = BlendFactor::One;

}; // class Pipeline

} // namespace Dusk

#endif // DUSK_PIPELINE_HPP