#include <Dusk/OpenGL/OpenGLBuffer.hpp>

#include <Dusk/Log.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLBuffer::Initialize(size_t size, uint8_t * data, BufferUsage usage)
{
    _size = size;

    _glTarget = GetGLBufferUsage(usage);
    if (_glTarget == GL_INVALID_ENUM) {
        return false;
    }

    glGenBuffers(1, &_glID);
    glBindBuffer(_glTarget, _glID);
    glBufferStorage(_glTarget, _size, data, 0);
    glBindBuffer(_glTarget, 0);

    return true;
}

DUSK_OPENGL_API
void OpenGLBuffer::Terminate()
{
    glDeleteBuffers(1, &_glID);
}

DUSK_OPENGL_API
void OpenGLBuffer::Bind()
{
    glBindBuffer(_glTarget, _glID);
}

} // namespace Dusk::OpenGL