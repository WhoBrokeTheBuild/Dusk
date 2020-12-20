#include <Dusk/OpenGL/OpenGLPrimitive.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
void OpenGLPrimitive::Render()
{
    glBindVertexArray(_glVAO);

    if (_indexed) {
        glDrawElements(_glMode, _glCount, GL_UNSIGNED_INT, NULL);
    }
    else {
        glDrawArrays(_glMode, 0, _glCount);
    }

    glBindVertexArray(0);
}

DUSK_OPENGL_API
bool OpenGLPrimitive::Load(const std::unique_ptr<PrimitiveData>& data)
{
    DuskBenchmarkStart();

    glGenVertexArrays(1, &_glVAO);
    glBindVertexArray(_glVAO);

    const auto& indexList = data->GetIndexList();
    const auto& vertexList = data->GetVertexList();

    _glMode = GetGLMode(data->GetTopology());

    GLuint vbo;
    std::vector<GLuint> vbos;

    if (indexList.empty()) {
        _indexed = false;
        _glCount = vertexList.size();
    }
    else {
        _indexed = true;
        _glCount = indexList.size();

        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, indexList.size() * sizeof(uint32_t), indexList.data(), 0);

        vbos.push_back(vbo);
    }

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferStorage(GL_ARRAY_BUFFER, vertexList.size() * sizeof(Vertex), vertexList.data(), 0);

    vbos.push_back(vbo);

    GLuint location;

    location = GetVertexAttributeLocation(VertexAttribute::Position);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));

    location = GetVertexAttributeLocation(VertexAttribute::Normal);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    location = GetVertexAttributeLocation(VertexAttribute::Tangent);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));

    location = GetVertexAttributeLocation(VertexAttribute::Bitangent);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

    location = GetVertexAttributeLocation(VertexAttribute::Color);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));

    location = GetVertexAttributeLocation(VertexAttribute::TexCoord1);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoord1));

    location = GetVertexAttributeLocation(VertexAttribute::TexCoord2);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoord2));

    location = GetVertexAttributeLocation(VertexAttribute::Joints);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_SHORT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Joints));

    location = GetVertexAttributeLocation(VertexAttribute::Weights);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Weights));

    glBindVertexArray(0);

    DuskBenchmarkEnd("OpenGLPrimitive::Load");
    return true;
}

DUSK_OPENGL_API
GLenum OpenGLPrimitive::GetGLMode(const PrimitiveTopology& topology)
{
    switch (topology) {
    case PrimitiveTopology::Points:
        return GL_POINTS;
    case PrimitiveTopology::Lines:
        return GL_LINES;
    case PrimitiveTopology::LineStrip:
        return GL_LINE_STRIP;
    case PrimitiveTopology::Triangles:
        return GL_TRIANGLES;
    case PrimitiveTopology::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    }

    return GL_INVALID_ENUM;
}

} // namespace Dusk::OpenGL
