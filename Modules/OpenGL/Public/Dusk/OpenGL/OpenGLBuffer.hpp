#ifndef DUSK_OPENGL_BUFFER_HPP
#define DUSK_OPENGL_BUFFER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>
#include <Dusk/Buffer.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API OpenGLBuffer : public Buffer
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLBuffer)

    OpenGLBuffer() = default;

    virtual inline ~OpenGLBuffer() {
        Terminate();
    }

    bool Initialize(size_t size, uint8_t * data, BufferUsage usage) override;

    void Terminate() override;

    void Bind();

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

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_BUFFER_HPP