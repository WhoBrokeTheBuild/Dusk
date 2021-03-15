#include <Dusk/OpenGL/OpenGLPrimitive.hpp>
#include <Dusk/OpenGL/OpenGLMaterial.hpp>
#include <Dusk/GraphicsDriver.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
void OpenGLPrimitive::Render()
{
    glBindVertexArray(_glVAO);

    OpenGLMaterial * glMaterial = DUSK_OPENGL_MATERIAL(_material.get());
    if (glMaterial) {
        glMaterial->Bind();
    }

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

    auto gfx = GetGraphicsDriver();

    _material = data->GetMaterial();
    if (!_material) {
        _material = gfx->GetDefaultMaterial();
    }

    glGenVertexArrays(1, &_glVAO);
    glBindVertexArray(_glVAO);

    const auto& indexList = data->GetIndexList();
    const auto& vertexList = data->GetVertexList();

    _glMode = GetGLPrimitiveTopology(data->GetTopology());

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

    DuskBenchmarkEnd();
    return true;
}

} // namespace Dusk::OpenGL
