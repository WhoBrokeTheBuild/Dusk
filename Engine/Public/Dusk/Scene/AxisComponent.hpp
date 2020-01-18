#ifndef DUSK_AXIS_COMPONENT_HPP
#define DUSK_AXIS_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Scene/Component.hpp>
#include <Dusk/Graphics/Mesh.hpp>
#include <Dusk/Graphics/Shader.hpp>

namespace Dusk {

class DUSK_CORE_API AxisMeshData : public IMeshData
{
public:

    const unsigned Indices[6] = {
        0, 1, 
        2, 3, 
        4, 5,
    };

    const float Vertices[24] = { 
        0.f, 0.f, 0.f, 1.0f,
        1.f, 0.f, 0.f, 1.0f,
        0.f, 0.f, 0.f, 1.0f,
        0.f, 1.f, 0.f, 1.0f,
        0.f, 0.f, 0.f, 1.0f,
        0.f, 0.f, 1.f, 1.0f,
    };

    const float Colors[24] = { 
        1.f, 0.f, 0.f, 1.0f,
        1.f, 0.f, 0.f, 1.0f,
        0.f, 1.f, 0.f, 1.0f,
        0.f, 1.f, 0.f, 1.0f,
        0.f, 0.f, 1.f, 1.0f,
        0.f, 0.f, 1.f, 1.0f,
    };

    Mode GetMode() const override {
        return Mode::Lines;
    }

    gsl::span<const unsigned> GetIndices() const override {
        return gsl::span(Indices, 6);
    }

    gsl::span<const float> GetVertices() const override {
        return gsl::span(Vertices, 24);
    }

    gsl::span<const float> GetNormals() const override {
        return gsl::span<float>();
    }

    gsl::span<const float> GetUVs() const override {
        return gsl::span<float>();
    }

    gsl::span<const float> GetColors() const override {
        return gsl::span(Colors, 24);
    }

    gsl::span<const float> GetTangents() const override {
        return gsl::span<float>();
    }

    gsl::span<const float> GetBitangents() const override {
        return gsl::span<float>();
    }

}; // class MeshData

class DUSK_CORE_API AxisComponent : public IComponent
{
public:

    DISALLOW_COPY_AND_ASSIGN(AxisComponent)

    explicit AxisComponent();

    virtual ~AxisComponent() = default;

    void Render(RenderContext * ctx) override;

private:

    static AxisMeshData _MeshData;

    std::unique_ptr<IShader> _shader;

    std::unique_ptr<IMesh> _mesh;

}; // class AxisComponent

} // namespace Dusk

#endif // DUSK_AXIS_COMPONENT_HPP