#include "dusk/asset/Texture.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>

#include <stb/stb_image.h>

#include <algorithm>

namespace dusk {

Texture::Texture(const std::string& filename, Options opts /*= Options()*/)
{
    LoadFromFile(filename, opts);
}

Texture::Texture(const uint8_t * data, glm::ivec2 size, int comp /*= 4*/, Options opts /*= Options()*/)
{
    LoadFromBuffer(data, size, comp, opts);
}

Texture::Texture(GLuint&& id, glm::ivec2 size)
{
    std::swap(_glID, id);
    _size = size;
    if (_glID >= 0) 
    {
        SetLoaded(true);
        DuskLogLoad("Loaded from existing ID %u", _glID);
    }
}

Texture::Texture(Texture&& rhs)
{
    std::swap(_glID, rhs._glID);
}

Texture::~Texture() 
{
    if (_glID > 0) {
        glDeleteTextures(1, &_glID);
    }
}

bool Texture::LoadFromFile(const std::string& filename, Options opts /*= Options()*/)
{
    DuskBenchStart();

    const auto& paths = GetAssetPaths();

    int comp;
    uint8_t * texture = nullptr;

    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath);
        texture = stbi_load(fullPath.c_str(), &_size.x, &_size.y, &comp, STBI_rgb_alpha);

        if (texture) break;
    }

    if (!texture) {
        DuskLogError("Failed to load texture '%s'", filename);
        return false;
    }

    bool loaded = LoadFromBuffer(texture, _size, comp, opts);

    stbi_image_free(texture);

    DuskBenchEnd("Texture::LoadFromFile");
    return loaded;
}

bool Texture::LoadFromBuffer(const uint8_t * buffer, glm::ivec2 size, int comp /*= 4*/, Options opts /*= Options()*/)
{
    DuskBenchStart();

    if (_glID > 0) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    glGenTextures(1, &_glID);

    if (0 == _glID) {
        DuskLogError("Failed to create GL Texture");
        return false;
    }

    GLint intfmt;
    GLenum fmt;

    switch (comp) 
    {
    case 1:
        intfmt = GL_RED;
        fmt = GL_RED;
        break;
    case 2:
        intfmt = GL_RG;
        fmt = GL_RG;
        break;
    case 3:
        intfmt = GL_RGB;
        fmt = GL_RGB;
        break;
    case 4:
    default:
        intfmt = GL_RGBA;
        fmt = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, _glID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    DuskLogVerbose("Binding texture to ID %u", _glID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.WrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.MinFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, intfmt, size.x, size.y, 0, fmt, GL_UNSIGNED_BYTE, buffer);

    if (opts.Mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    SetLoaded(true);

    DuskBenchEnd("Texture::LoadFromBuffer");
    return true;
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _glID);
}

} // namespace dusk
