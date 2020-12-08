#include <Dusk/OpenGL/OpenGLMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk {

DUSK_OPENGL_API
void OpenGLMesh::Render()
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
bool OpenGLMesh::Load(const MeshData * data)
{
    DuskBenchmarkStart();

    glGenVertexArrays(1, &_glVAO);
    glBindVertexArray(_glVAO);

    const auto& indices = data->GetIndices();
    const auto& vertices = data->GetVertices();
    const auto& normals = data->GetNormals();
    const auto& uvs = data->GetUVs();
    const auto& colors = data->GetColors();

    _glMode = GetGLMode(data->GetMode());

    GLuint vbo;
    std::vector<GLuint> vbos;

    if (indices.empty()) {
        _indexed = false;
        _glCount = vertices.size();
    }
    else {
        _indexed = true;
        _glCount = indices.size();

        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

        vbos.push_back(vbo);
    }

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Position);
    glVertexAttribPointer((GLuint)VertexAttributeLocation::Position, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

    vbos.push_back(vbo);

    if (!normals.empty()) {
        glGenBuffers(1, &vbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Normal);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::Normal, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }

    if (!colors.empty()) {
        glGenBuffers(1, &vbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::Color1);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::Color1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

        vbos.push_back(vbo);
    }
    
    if (!uvs.empty()) {
        glGenBuffers(1, &vbo);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray((GLuint)VertexAttributeLocation::UV1);
        glVertexAttribPointer((GLuint)VertexAttributeLocation::UV1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        vbos.push_back(vbo);
    }

    glBindVertexArray(0);

    DuskBenchmarkEnd("OpenGLMesh::Load");
    return true;
}

DUSK_OPENGL_API
GLenum OpenGLMesh::GetGLMode(const MeshData::Mode& mode)
{
    switch (mode) {
    case MeshData::Mode::Points:
        return GL_POINTS;
    case MeshData::Mode::Lines:
        return GL_LINES;
    case MeshData::Mode::Triangles:
        return GL_TRIANGLES;
    case MeshData::Mode::TriangleFan:
        return GL_TRIANGLE_FAN;
    }

    return GL_INVALID_ENUM;
}

} // namespace Dusk
