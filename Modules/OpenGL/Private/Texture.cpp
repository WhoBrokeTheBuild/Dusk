#include <Dusk/OpenGL/Texture.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk {

DUSK_OPENGL_API
OpenGLTexture::~OpenGLTexture()
{
    if (_glID) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }
}

DUSK_OPENGL_API
bool OpenGLTexture::Load(const TextureData * data, Options opts /*= Options()*/)
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

    GLenum glFormat = GetGLDataFormat(data->GetComponents());
    GLenum glType = GetGLDataType(data->GetDataType());

    glBindTexture(GL_TEXTURE_2D, _glID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrapType(opts.WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrapType(opts.WrapT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterType(opts.MagFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterType(opts.MinFilter));

    const auto& size = data->GetSize();
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)glFormat, size.x, size.y, 0, glFormat, glType, data->GetData());

    if (opts.GenerateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    DuskLogVerbose("Bound texture to ID %d", _glID);

    glBindTexture(GL_TEXTURE_2D, 0);

    DuskBenchmarkEnd("OpenGLTexture::Load");
    return true;
}

DUSK_OPENGL_API
void OpenGLTexture::Bind() 
{
    glBindTexture(GL_TEXTURE_2D, _glID);
}

DUSK_OPENGL_API
GLenum OpenGLTexture::GetGLDataFormat(int components)
{
    switch (components) {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    }

    return GL_INVALID_ENUM;
}

DUSK_OPENGL_API
GLenum OpenGLTexture::GetGLDataType(const TextureData::DataType& type)
{
    switch (type) {
    case TextureData::DataType::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case TextureData::DataType::Byte:
        return GL_BYTE;
    case TextureData::DataType::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case TextureData::DataType::Short:
        return GL_SHORT;
    case TextureData::DataType::UnsignedInt:
        return GL_UNSIGNED_INT;
    case TextureData::DataType::Int:
        return GL_INT;
    }

    return GL_INVALID_ENUM;
}

DUSK_OPENGL_API
GLenum OpenGLTexture::GetGLWrapType(const Texture::WrapType& type)
{
    switch (type) {
    case Texture::WrapType::Repeat:
        return GL_REPEAT;
    case Texture::WrapType::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case Texture::WrapType::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case Texture::WrapType::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return GL_INVALID_ENUM;
}

DUSK_OPENGL_API
GLenum OpenGLTexture::GetGLFilterType(const Texture::FilterType& type)
{
    switch (type) {
    case Texture::FilterType::Nearest:
        return GL_NEAREST;
    case Texture::FilterType::NearestMipmapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case Texture::FilterType::NearestMipmapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    case Texture::FilterType::Linear:
        return GL_LINEAR;
    case Texture::FilterType::LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    case Texture::FilterType::LinearMipmapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    }

    return GL_INVALID_ENUM;
}

} // namespace Dusk
