#include <Dusk/OpenGL/OpenGLBuffer.hpp>
#include <Dusk/Exception.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    _size = size;
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;

    if (!data && memoryUsage == MemoryUsage::GPU) {
        throw InvalidArgument("data, memoryUsage", "Attempting to create a buffer with no data and MemoryUsage::GPU");
    }
    
    _glTarget = GetGLBufferUsage(bufferUsage);
    if (_glTarget == GL_INVALID_ENUM) {
        throw InvalidArgument("bufferUsage", "Unable to map BufferUsage::{} to OpenGL Target", BufferUsageToString(bufferUsage));
    }

    GLbitfield flags = GetGLMemoryUsage(memoryUsage);

    glGenBuffers(1, &_glID);
    glBindBuffer(_glTarget, _glID);
    glBufferStorage(_glTarget, _size, data, flags);

    // TODO: Limit to only mappable memory usages
    _mappedBufferMemory = glMapBufferRange(_glTarget, 0, _size, flags);
    if (!_mappedBufferMemory) {
        throw Exception("glMapBufferRange() failed");
    }

    glBindBuffer(_glTarget, 0);
    return true;
}

DUSK_OPENGL_API
void OpenGLBuffer::Terminate()
{
    if (_mappedBufferMemory) {
        glBindBuffer(_glTarget, _glID);
        glUnmapBuffer(_glTarget);
        glBindBuffer(_glTarget, 0);
    }

    if (_glID > 0) {
        glDeleteBuffers(1, &_glID);
    }
}

DUSK_OPENGL_API
void OpenGLBuffer::Bind()
{
    glBindBuffer(_glTarget, _glID);
}

} // namespace Dusk::OpenGL
