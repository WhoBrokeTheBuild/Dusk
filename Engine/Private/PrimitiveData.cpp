#include <Dusk/PrimitiveData.hpp>

#include <Dusk/Log.hpp>

namespace Dusk {

void PrimitiveData::CalculateTangents()
{
    auto topology = GetTopology();
    auto indexList = GetIndexList();
    auto vertexList = GetVertexList();

    auto calcVertex = [](Vertex& v1, Vertex& v2, Vertex& v3)
    {
        vec3 v = vec3(v2.Position) - vec3(v1.Position);
        vec3 w = vec3(v3.Position) - vec3(v1.Position);
        vec2 s = v2.TexCoord1 - v1.TexCoord1;
        vec2 t = v3.TexCoord1 - v1.TexCoord1;

        float dir = 1.0f;
        if ((t.x * s.y - t.y * s.x) < 0.0f) {
            dir = -1.0f;
        }

        glm::vec3 tangent = {
            dir * (w.x * s.y - v.x * t.y),
            dir * (w.y * s.y - v.y * t.y),
            dir * (w.z * s.y - v.z * t.y),
        };

        v1.Tangent = normalize(vec4(tangent - vec3(v1.Normal) * (tangent * vec3(v1.Normal)), 1.0f));
        v2.Tangent = normalize(vec4(tangent - vec3(v2.Normal) * (tangent * vec3(v2.Normal)), 1.0f));
        v3.Tangent = normalize(vec4(tangent - vec3(v3.Normal) * (tangent * vec3(v3.Normal)), 1.0f));

        // TODO: Account for NaN and smoothing
    };

    if (topology == PrimitiveTopology::TriangleList) {
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
    else if (topology == PrimitiveTopology::TriangleStrip) {
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