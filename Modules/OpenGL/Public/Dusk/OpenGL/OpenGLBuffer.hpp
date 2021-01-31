#ifndef DUSK_OPENGL_BUFFER_HPP
#define DUSK_OPENGL_BUFFER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>
#include <Dusk/Buffer.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_BUFFER(x) (dynamic_cast<Dusk::OpenGL::OpenGLBuffer *>(x))

class DUSK_OPENGL_API OpenGLBuffer : public Buffer
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLBuffer)

    OpenGLBuffer() = default;

    virtual inline ~OpenGLBuffer() {
        Terminate();
    }

    bool Initialize(size_t size, uint8_t * data, BufferUsage bufferUsage, MemoryUsage memoryUsage) override;

    void Terminate() override;

    bool MapBufferToMemory() override;

    void Bind();

    GLuint GetGLID() const {
        return _glID;
    }

private:

    GLuint _glID = 0;

    GLenum _glTarget;

    size_t _size;

}; // class OpenGLBuffer

inline GLenum GetGLBufferUsage(BufferUsage bufferUsage)
{
    switch (bufferUsage) {
    case BufferUsage::Index:
        return GL_ELEMENT_ARRAY_BUFFER;
    case BufferUsage::Vertex:
        return GL_ARRAY_BUFFER;
    case BufferUsage::Constant:
        return GL_UNIFORM_BUFFER;
    }

    return GL_INVALID_ENUM;
}

inline GLbitfield GetGLMemoryUsage(MemoryUsage memoryUsage)
{
    switch (memoryUsage) {
    case MemoryUsage::GPU:
        return 0;
    case MemoryUsage::UploadOnce:
    case MemoryUsage::UploadOften:
        return GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
    case MemoryUsage::Download:
        return GL_MAP_READ_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
    }

    return 0;
}

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_BUFFER_HPP