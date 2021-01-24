#include <Dusk/OpenGL/OpenGLMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>
#include <Dusk/OpenGL/OpenGLPrimitive.hpp>
#include <Dusk/OpenGL/OpenGLPipeline.hpp>
#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    for (const auto& primitiveData : data) {
        std::unique_ptr<Primitive> primitive = std::unique_ptr<Primitive>(New OpenGLPrimitive());
        if (!primitive->Load(primitiveData)) {
            return false;
        }

        _primitiveList.push_back(std::move(primitive));
    }

    return true;
}

DUSK_OPENGL_API
void OpenGLMesh::Render(RenderContext * ctx)
{
    OpenGLPipeline * glPipeline = DUSK_OPENGL_PIPELINE(_pipeline.get());
    glPipeline->Bind();

    for (const auto& primitive : _primitiveList) {
        OpenGLPrimitive * glPrimitive = DUSK_OPENGL_PRIMITIVE(primitive.get());
        glPrimitive->Render();
    }
}

} // namespace Dusk::OpenGL
