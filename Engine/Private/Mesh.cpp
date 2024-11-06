#include <Dusk/Mesh.hpp>

namespace dusk {

void Mesh::AddPlane(
        vec3 center /*= { 0.0f, 0.0f, 0.0f } */,
        vec3 normal /*= { 0.0f, 1.0f, 0.0f } */,
        vec2 size /*= { 1.0f, 1.0f } */,
        uvec2 subdivisions /*= { 1, 1 } */
) {
    unsigned vertexCount = (subdivisions.x + 1) * (subdivisions.y + 1);
    List<PrimitiveVertex> vertexList(vertexCount);

    vec3 topLeft = {
        center.x - (size.x * 0.5f),
        center.y - (size.y * 0.5f),
        center.z
    };

    vec2 step = size / vec2(subdivisions);

    for (unsigned row = 0; row < subdivisions.x; ++row) {
        for (unsigned col = 0; col < subdivisions.y; ++col) {
            unsigned index = (row * subdivisions.x) + col;
            vertexList[index].Position = {
                topLeft.x + (step.x * row),
                topLeft.y + (step.y * col),
                topLeft.z,
                1.0f,
            };
        }
    }
    
    
    VulkanPrimitive::Pointer primitive(new VulkanPrimitive());
    primitive->Create(vertexList);
    AddPrimitive(std::move(primitive));
}

} // namespace dusk