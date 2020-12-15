#include <Dusk/OpenGL/OpenGLMesh.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
void OpenGLMesh::Render()
{
    for (const auto& primitive : _primitives) {
        primitive->Render();
    }
}

DUSK_OPENGL_API
bool OpenGLMesh::Load(const std::vector<std::unique_ptr<PrimitiveData>>& data)
{
    for (const auto& primitiveData : data) {
        _primitives.push_back(std::make_unique<OpenGLPrimitive>());
        if (!_primitives.back()->Load(primitiveData)) {
            return false;
        }
    }

    return true;
}

} // namespace Dusk::OpenGL
