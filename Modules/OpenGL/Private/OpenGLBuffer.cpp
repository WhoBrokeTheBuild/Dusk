#include <Dusk/OpenGL/OpenGLBuffer.hpp>

#include <Dusk/Log.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = size;

    _glTarget = GetGLBufferUsage(_bufferUsage);
    if (_glTarget == GL_INVALID_ENUM) {
        return false;
    }

    GLbitfield flags = GetGLMemoryUsage(memoryUsage);

    glGenBuffers(1, &_glID);
    glBindBuffer(_glTarget, _glID);
    glBufferStorage(_glTarget, _size, data, flags);
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