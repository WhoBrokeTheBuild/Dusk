#ifndef DUSK_AXIS_COMPONENT_HPP
#define DUSK_AXIS_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Component.hpp>
#include <Dusk/Mesh.hpp>
#include <Dusk/Shader.hpp>

namespace Dusk {

// class DUSK_ENGINE_API AxisPrimitiveData : public PrimitiveData
// {
// public:

//     const unsigned Indices[6] = {
//         0, 1, 
//         2, 3, 
//         4, 5,
//     };

//     const float Vertices[24] = { 
//         0.f, 0.f, 0.f, 1.0f,
//         1.f, 0.f, 0.f, 1.0f,
//         0.f, 0.f, 0.f, 1.0f,
//         0.f, 1.f, 0.f, 1.0f,
//         0.f, 0.f, 0.f, 1.0f,
//         0.f, 0.f, 1.f, 1.0f,
//     };

//     const float Colors[24] = { 
//         1.f, 0.f, 0.f, 1.0f,
//         1.f, 0.f, 0.f, 1.0f,
//         0.f, 1.f, 0.f, 1.0f,
//         0.f, 1.f, 0.f, 1.0f,
//         0.f, 0.f, 1.f, 1.0f,
//         0.f, 0.f, 1.f, 1.0f,
//     };

//     Mode GetMode() const override {
//         return Mode::Lines;
//     }

//     std::span<const unsigned> GetIndices() const override {
//         return std::span(Indices, 6);
//     }

//     std::span<const float> GetVertices() const override {
//         return std::span(Vertices, 24);
//     }

//     std::span<const float> GetNormals() const override {
//         return std::span<float>();
//     }

//     std::span<const float> GetUVs() const override {
//         return std::span<float>();
//     }

//     std::span<const float> GetColors() const override {
//         return std::span(Colors, 24);
//     }

//     std::span<const float> GetTangents() const override {
//         return std::span<float>();
//     }

//     std::span<const float> GetBitangents() const override {
//         return std::span<float>();
//     }

// }; // class PrimitiveData

class DUSK_ENGINE_API AxisComponent : public Component
{
public:

    DISALLOW_COPY_AND_ASSIGN(AxisComponent)

    AxisComponent();

    virtual ~AxisComponent() = default;

    void Render(RenderContext * ctx) override;

private:

    // static AxisPrimitiveData _PrimitiveData;

    std::shared_ptr<Shader> _shader;

    std::shared_ptr<Mesh> _mesh;

}; // class AxisComponent

} // namespace Dusk

#endif // DUSK_AXIS_COMPONENT_HPP