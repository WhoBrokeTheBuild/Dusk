#include <Dusk/OpenGL/OpenGLBuffer.hpp>

#include <Dusk/Log.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLBuffer::Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage)
{
    _bufferUsage = bufferUsage;
    _memoryUsage = memoryUsage;
    _size = size;

    if (!data && _memoryUsage == MemoryUsage::GPU) {
        DuskLogError("Attempting to create an empty buffer with MemoryUsage GPU");
        return false;
    }
    
    _glTarget = GetGLBufferUsage(_bufferUsage);
    if (_glTarget == GL_INVALID_ENUM) {
        return false;
    }

    GLbitfield flags = GetGLMemoryUsage(_memoryUsage);

    glGenBuffers(1, &_glID);
    glBindBuffer(_glTarget, _glID);
    glBufferStorage(_glTarget, _size, data, flags);

    if (!MapBufferToMemory()) {
        return false;
    }

    return true;
}

DUSK_OPENGL_API
void OpenGLBuffer::Terminate()
{
    if (_mappedBufferMemory) {
        glBindBuffer(_glTarget, _glID);
        glUnmapBuffer(_glTarget);
        glBindBuffer(_glTarget, 0);
        
        _mappedBufferMemory = nullptr;
    }

    glDeleteBuffers(1, &_glID);
}

DUSK_OPENGL_API
bool OpenGLBuffer::MapBufferToMemory()
{
    glBindBuffer(_glTarget, _glID);
    
    GLenum access = GL_INVALID_ENUM;

    if (_memoryUsage == MemoryUsage::UploadOnce ||
        _memoryUsage == MemoryUsage::UploadOften) {
        access = GL_WRITE_ONLY;
    }
    else if (_memoryUsage == MemoryUsage::Download) {
        access = GL_READ_ONLY;
    }

    _mappedBufferMemory = glMapBuffer(_glTarget, access);
    if (!_mappedBufferMemory) {
        DuskLogError("glMapBuffer() failed");
        return false;
    }

    glBindBuffer(_glTarget, 0);
    return true;
}

DUSK_OPENGL_API
void OpenGLBuffer::Bind()
{
    glBindBuffer(_glTarget, _glID);
}

} // namespace Dusk::OpenGL
