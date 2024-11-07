#include <Dusk/Mesh.hpp>
#include <Dusk/Color.hpp>

namespace dusk {

void Mesh::AddCube(
    vec3 center /*= { 0.0f, 0.0f, 0.0f }*/,
    vec3 size /*= { 1.0f, 1.0f, 1.0f }*/,
    Material::Pointer material /*= Graphics::DefaultMaterial*/
) {
    vec3 halfSize = size * 0.5f;

    vec4 topBackLeft = { center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 topBackRight = { center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 topFrontLeft = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f };
    vec4 topFrontRight = { center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f };
    vec4 bottomBackLeft = { center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 bottomBackRight = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 bottomFrontLeft = { center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f };
    vec4 bottomFrontRight = { center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f };

    constexpr vec4 up = { 0.0f, 1.0f, 0.0f, 1.0f };
    constexpr vec4 down = { 0.0f, -1.0f, 0.0f, 1.0f };
    constexpr vec4 left = { 1.0f, 0.0f, 0.0f, 1.0f };
    constexpr vec4 right = { -1.0f, 0.0f, 0.0f, 1.0f };
    constexpr vec4 forward = { 0.0f, 0.0f, 1.0f, 1.0f };
    constexpr vec4 backward = { 0.0f, 0.0f, -1.0f, 1.0f };

    PrimitiveVertex vertexList[] = {
        // Top
        PrimitiveVertex{ .Position = topFrontLeft, .Normal = up, },
        PrimitiveVertex{ .Position = topBackLeft, .Normal = up, },
        PrimitiveVertex{ .Position = topFrontRight, .Normal = up, },
        PrimitiveVertex{ .Position = topBackRight, .Normal = up, },
        PrimitiveVertex{ .Position = topFrontRight, .Normal = up, },
        PrimitiveVertex{ .Position = topBackLeft, .Normal = up, },
        // Bottom
        PrimitiveVertex{ .Position = bottomFrontLeft, .Normal = down, },
        PrimitiveVertex{ .Position = bottomFrontRight, .Normal = down, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Normal = down, },
        PrimitiveVertex{ .Position = bottomBackRight, .Normal = down, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Normal = down, },
        PrimitiveVertex{ .Position = bottomFrontRight, .Normal = down, },
        // Left
        PrimitiveVertex{ .Position = bottomFrontLeft, .Normal = left, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Normal = left, },
        PrimitiveVertex{ .Position = topFrontLeft, .Normal = left, },
        PrimitiveVertex{ .Position = topBackLeft, .Normal = left, },
        PrimitiveVertex{ .Position = topFrontLeft, .Normal = left, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Normal = left, },
        // Right
        PrimitiveVertex{ .Position = bottomFrontRight, .Normal = right, },
        PrimitiveVertex{ .Position = topFrontRight, .Normal = right, },
        PrimitiveVertex{ .Position = bottomBackRight, .Normal = right, },
        PrimitiveVertex{ .Position = topBackRight, .Normal = right, },
        PrimitiveVertex{ .Position = bottomBackRight, .Normal = right, },
        PrimitiveVertex{ .Position = topFrontRight, .Normal = right, },
        // Back
        PrimitiveVertex{ .Position = bottomBackLeft, .Normal = backward, },
        PrimitiveVertex{ .Position = bottomBackRight, .Normal = backward, },
        PrimitiveVertex{ .Position = topBackRight, .Normal = backward, },
        PrimitiveVertex{ .Position = topBackRight, .Normal = backward, },
        PrimitiveVertex{ .Position = topBackLeft, .Normal = backward, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Normal = backward, },
        // Front
        PrimitiveVertex{ .Position = topFrontRight, .Normal = forward, },
        PrimitiveVertex{ .Position = bottomFrontRight, .Normal = forward, },
        PrimitiveVertex{ .Position = bottomFrontLeft, .Normal = forward, },
        PrimitiveVertex{ .Position = bottomFrontLeft, .Normal = forward, },
        PrimitiveVertex{ .Position = topFrontLeft, .Normal = forward, },
        PrimitiveVertex{ .Position = topFrontRight, .Normal = forward, },
    };

    // Fill vertex list

    VulkanPrimitive::Pointer primitive(new VulkanPrimitive());
    primitive->Create(
        vertexList,
        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        false
    );
    primitive->SetMaterial(material);
    AddPrimitive(std::move(primitive));
}

void Mesh::AddWireCube(
    vec3 center /*= { 0.0f, 0.0f, 0.0f }*/,
    vec3 size /*= { 1.0f, 1.0f, 1.0f }*/,
    vec4 color /*= { 0.0f, 0.0f, 0.0f, 1.0f }*/,
    Material::Pointer material /*= Graphics::DefaultMaterial*/
) {
    vec3 halfSize = size * 0.5f;

    vec4 topBackLeft = { center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 topBackRight = { center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 topFrontLeft = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f };
    vec4 topFrontRight = { center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z, 1.0f };
    vec4 bottomBackLeft = { center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 bottomBackRight = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z, 1.0f };
    vec4 bottomFrontLeft = { center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f };
    vec4 bottomFrontRight = { center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z, 1.0f };

    PrimitiveVertex vertexList[] = {
        PrimitiveVertex{ .Position = topFrontLeft, .Color = color, },
        PrimitiveVertex{ .Position = topBackLeft, .Color = color, },

        PrimitiveVertex{ .Position = topBackLeft, .Color = color, },
        PrimitiveVertex{ .Position = topBackRight, .Color = color, },

        PrimitiveVertex{ .Position = topBackRight, .Color = color, },
        PrimitiveVertex{ .Position = topFrontRight, .Color = color, },

        PrimitiveVertex{ .Position = topFrontRight, .Color = color, },
        PrimitiveVertex{ .Position = topFrontLeft, .Color = color, },

        PrimitiveVertex{ .Position = bottomFrontLeft, .Color = color, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Color = color, },

        PrimitiveVertex{ .Position = bottomBackLeft, .Color = color, },
        PrimitiveVertex{ .Position = bottomBackRight, .Color = color, },

        PrimitiveVertex{ .Position = bottomBackRight, .Color = color, },
        PrimitiveVertex{ .Position = bottomFrontRight, .Color = color, },

        PrimitiveVertex{ .Position = bottomFrontRight, .Color = color, },
        PrimitiveVertex{ .Position = bottomFrontLeft, .Color = color, },

        PrimitiveVertex{ .Position = topFrontLeft, .Color = color, },
        PrimitiveVertex{ .Position = bottomFrontLeft, .Color = color, },

        PrimitiveVertex{ .Position = topFrontRight, .Color = color, },
        PrimitiveVertex{ .Position = bottomFrontRight, .Color = color, },

        PrimitiveVertex{ .Position = topBackLeft, .Color = color, },
        PrimitiveVertex{ .Position = bottomBackLeft, .Color = color, },

        PrimitiveVertex{ .Position = topBackRight, .Color = color, },
        PrimitiveVertex{ .Position = bottomBackRight, .Color = color, },
    };

    // Fill vertex list

    VulkanPrimitive::Pointer primitive(new VulkanPrimitive());
    primitive->Create(
        vertexList,
        VK_PRIMITIVE_TOPOLOGY_LINE_LIST
    );
    primitive->SetMaterial(material); // TODO:
    AddPrimitive(std::move(primitive));
}

// void Mesh::AddPlane(
//         vec3 center /*= { 0.0f, 0.0f, 0.0f } */,
//         vec3 normal /*= { 0.0f, 1.0f, 0.0f } */,
//         vec2 size /*= { 1.0f, 1.0f } */,
//         uvec2 subdivisions /*= { 1, 1 } */
// ) {
//     unsigned vertexCount = (subdivisions.x + 1) * (subdivisions.y + 1);
//     List<PrimitiveVertex> vertexList(vertexCount);

//     vec3 topLeft = {
//         center.x - (size.x * 0.5f),
//         center.y - (size.y * 0.5f),
//         center.z
//     };

//     vec2 step = size / vec2(subdivisions);

//     for (unsigned row = 0; row < subdivisions.x; ++row) {
//         for (unsigned col = 0; col < subdivisions.y; ++col) {
//             unsigned index = (row * subdivisions.x) + col;
//             vertexList[index].Position = {
//                 topLeft.x + (step.x * row),
//                 topLeft.y + (step.y * col),
//                 topLeft.z,
//                 1.0f,
//             };
//         }
//     }
    
    
//     VulkanPrimitive::Pointer primitive(new VulkanPrimitive());
//     primitive->Create(vertexList);
//     AddPrimitive(std::move(primitive));
// }

} // namespace dusk