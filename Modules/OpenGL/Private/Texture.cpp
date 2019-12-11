#include <Dusk/OpenGL/Texture.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::OpenGL {

Texture::~Texture()
{
    glDeleteTextures(1, &_glID);
}

DUSK_OPENGL_API
bool Texture::Load(const TextureData& data)
{
    DuskBenchmarkStart();

    if (_glID) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    glGenTextures(1, &_glID);
    if (!_glID) {
        DuskLogError("Failed to create GL Texture");
        return false;
    }

    GLenum format = GetGLDataFormat(data.DataFormat);
    GLenum type = GetGLDataType(data.DataType);

    glBindTexture(GL_TEXTURE_2D, _glID);
    
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.WrapS);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.WrapT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.MagFilter);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.MinFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, data.Size.x, data.Size.y, 0, format, type, data.Buffer);

    DuskLogVerbose("Bound texture to %d", _glID);

    glBindTexture(GL_TEXTURE_2D, 0);

    DuskBenchmarkEnd("OpenGL::Texture::Load");
    return true;
}

DUSK_OPENGL_API
void Texture::Bind() 
{
    glBindTexture(GL_TEXTURE_2D, _glID);
}

DUSK_OPENGL_API
GLenum Texture::GetGLDataFormat(const TextureDataFormat& format)
{
    switch (format) {
    case TextureDataFormat::R:
        return GL_RED;
    case TextureDataFormat::RG:
        return GL_RG;
    case TextureDataFormat::RGB:
        return GL_RGB;
    case TextureDataFormat::RGBA:
        return GL_RGBA;
    }

    return GL_INVALID_ENUM;
}

DUSK_OPENGL_API
GLenum Texture::GetGLDataType(const TextureDataType& type)
{
    switch (type) {
    case TextureDataType::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case TextureDataType::Byte:
        return GL_BYTE;
    case TextureDataType::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case TextureDataType::Short:
        return GL_SHORT;
    case TextureDataType::UnsignedInt:
        return GL_UNSIGNED_INT;
    case TextureDataType::Int:
        return GL_INT;
    }

    return GL_INVALID_ENUM;
}

} // namespace Dusk::OpenGL
