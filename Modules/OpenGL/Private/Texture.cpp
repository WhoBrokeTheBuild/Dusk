#include <Dusk/OpenGL/Texture.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Benchmark.hpp>

namespace Dusk::OpenGL {

Texture::~Texture()
{
    glDeleteTextures(1, &_glID);
}

DUSK_OPENGL_API
bool Texture::Load(const ITextureData * data)
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
    

    const auto& wrap = data->GetWrapTypes();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrapType(std::get<0>(wrap)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrapType(std::get<1>(wrap)));

    const auto& filter = data->GetFilterTypes();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterType(std::get<0>(filter)));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterType(std::get<1>(filter)));

    const auto& size = data->GetSize();
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)glFormat, size.x, size.y, 0, glFormat, glType, data->GetData());

    if (data->GenerateMipmaps()) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

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
GLenum Texture::GetGLDataFormat(int components)
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
GLenum Texture::GetGLDataType(const ITextureData::DataType& type)
{
    switch (type) {
    case ITextureData::DataType::UnsignedByte:
        return GL_UNSIGNED_BYTE;
    case ITextureData::DataType::Byte:
        return GL_BYTE;
    case ITextureData::DataType::UnsignedShort:
        return GL_UNSIGNED_SHORT;
    case ITextureData::DataType::Short:
        return GL_SHORT;
    case ITextureData::DataType::UnsignedInt:
        return GL_UNSIGNED_INT;
    case ITextureData::DataType::Int:
        return GL_INT;
    }

    return GL_INVALID_ENUM;
}

DUSK_OPENGL_API
GLenum Texture::GetGLWrapType(const ITextureData::WrapType& type)
{
    switch (type) {
    case ITextureData::WrapType::Repeat:
        return GL_REPEAT;
    case ITextureData::WrapType::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case ITextureData::WrapType::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case ITextureData::WrapType::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return GL_INVALID_ENUM;
}

DUSK_OPENGL_API
GLenum Texture::GetGLFilterType(const ITextureData::FilterType& type)
{
    switch (type) {
    case ITextureData::FilterType::Nearest:
        return GL_NEAREST;
    case ITextureData::FilterType::Linear:
        return GL_LINEAR;
    }

    return GL_INVALID_ENUM;
}

} // namespace Dusk::OpenGL
