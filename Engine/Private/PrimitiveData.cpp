#include <Dusk/PrimitiveData.hpp>

#include <Dusk/Log.hpp>

namespace Dusk {

void PrimitiveData::CalculateTBN()
{
    auto topology = GetTopology();
    auto indexList = GetIndexList();
    auto vertexList = GetVertexList();

    auto calcVertex = [](Primitive::Vertex& v1, Primitive::Vertex& v2, Primitive::Vertex& v3)
    {
        vec3 v = vec3(v2.Position) - vec3(v1.Position);
        vec3 w = vec3(v3.Position) - vec3(v1.Position);
        vec2 s = v2.TexCoord1 - v1.TexCoord1;
        vec2 t = v3.TexCoord1 - v1.TexCoord1;

        float dir = 1.0f;
        if ((t.x * s.y - t.y * s.x) < 0.0f) {
            dir = -1.0f;
        }

        float f = 1.0f / (s.x * t.y - t.x * s.y);

        glm::vec3 tangent = {
            dir * (w.x * s.y - v.x * t.y),
            dir * (w.y * s.y - v.y * t.y),
            dir * (w.z * s.y - v.z * t.y),
        };
        
        glm::vec3 bitangent = {
            dir * (w.x * s.x - v.x * t.x),
            dir * (w.y * s.x - v.y * t.x),
            dir * (w.z * s.x - v.z * t.x),
        };

        v1.Tangent = vec4(tangent - vec3(v1.Normal) * (tangent * vec3(v1.Normal)), 1.0f);
        v2.Tangent = vec4(tangent - vec3(v2.Normal) * (tangent * vec3(v2.Normal)), 1.0f);
        v3.Tangent = vec4(tangent - vec3(v3.Normal) * (tangent * vec3(v3.Normal)), 1.0f);

        v1.Tangent = normalize(v1.Tangent);
        v2.Tangent = normalize(v2.Tangent);
        v3.Tangent = normalize(v3.Tangent);

        v1.Bitangent = vec4(bitangent - vec3(v1.Normal) * (bitangent * vec3(v1.Normal)), 1.0f);
        v2.Bitangent = vec4(bitangent - vec3(v2.Normal) * (bitangent * vec3(v2.Normal)), 1.0f);
        v3.Bitangent = vec4(bitangent - vec3(v3.Normal) * (bitangent * vec3(v3.Normal)), 1.0f);

        v1.Bitangent = normalize(v1.Bitangent);
        v2.Bitangent = normalize(v2.Bitangent);
        v3.Bitangent = normalize(v3.Bitangent);

        // TODO: Account for NaN and smoothing
    };

    if (topology == Primitive::Topology::Triangles) {
        if (indexList.empty()) {
            for (size_t i = 0; i < vertexList.size(); i += 3) {
                calcVertex(
                    vertexList[i + 0],
                    vertexList[i + 1],
                    vertexList[i + 2]
                );
            }
        }
        else {
            for (size_t i = 0; i < indexList.size(); i += 3) {
                calcVertex(
                    vertexList[indexList[i + 0]],
                    vertexList[indexList[i + 1]],
                    vertexList[indexList[i + 2]]
                );
            }
        }
    }
    else if (topology == Primitive::Topology::TriangleStrip) {
        if (indexList.empty()) {
            for (size_t i = 0; i < vertexList.size(); ++i) {
                calcVertex(
                    vertexList[i + 0],
                    vertexList[i + 1],
                    vertexList[i + 2]
                );
            }
        }
        else {
            for (size_t i = 0; i < indexList.size(); ++i) {
                calcVertex(
                    vertexList[indexList[i + 0]],
                    vertexList[indexList[i + 1]],
                    vertexList[indexList[i + 2]]
                );
            }
        }
    }
}

} // namespace Dusk