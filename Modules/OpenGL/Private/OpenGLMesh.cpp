#include <Dusk/OpenGL/OpenGLMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/OpenGL/OpenGLPrimitive.hpp>
#include <Dusk/OpenGL/OpenGLPipeline.hpp>
#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
void OpenGLMesh::Render(RenderContext * ctx)
{
    Mesh::Render(ctx);

    OpenGLPipeline * glPipeline = static_cast<OpenGLPipeline *>(_pipeline.get());
    glPipeline->Bind();

    OpenGLBuffer * glTransformBuffer = static_cast<OpenGLBuffer *>(_shaderTransformBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, DUSK_SHADER_TRANSFORM_BINDING, glTransformBuffer->GetGLID());

    for (const auto& primitive : _primitiveList) {
        OpenGLPrimitive * glPrimitive = static_cast<OpenGLPrimitive *>(primitive.get());
        glPrimitive->Render();
    }
}

} // namespace Dusk::OpenGL
