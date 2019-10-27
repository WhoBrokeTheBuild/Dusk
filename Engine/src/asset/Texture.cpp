#include "dusk/asset/Texture.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>

#include <algorithm>

#include <stb/stb_image.h>

namespace dusk {

Texture::Texture(const std::string& filename, Options opts /*= Options()*/)
{
    LoadFromFile(filename, opts);
}

Texture::Texture(const uint8_t * buffer, glm::ivec2 size, int comp /*= 4*/, Options opts /*= Options()*/)
{
    LoadFromBuffer(buffer, size, comp, opts);
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
    uint8_t * buffer = nullptr;

    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath);
        buffer = stbi_load(fullPath.c_str(), &_size.x, &_size.y, &comp, STBI_rgb_alpha);

        if (buffer) {
            break;
        }
    }

    if (!buffer) {
        DuskLogError("Failed to load texture '%s'", filename);
        return false;
    }

    bool loaded = LoadFromBuffer(buffer, _size, comp, opts);
    if (loaded) {
        DuskLogLoad("Loaded Texture from '%s", fullPath);
    }

    stbi_image_free(buffer);

    DuskBenchEnd("Texture::LoadFromFile");
    return loaded;
}

bool Texture::LoadFromBuffer(const uint8_t * buffer, glm::ivec2 size, int comp /*= 4*/, Options opts /*= Options()*/)
{
    DuskBenchStart();

    _size = size;

    if (_glID) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    glGenTextures(1, &_glID);

    if (!_glID) {
        DuskLogError("Failed to create GL Texture");
        return false;
    }

    GLenum fmt = GL_RGBA;

    switch (comp) 
    {
    case 1:
        fmt = GL_RED;
        break;
    case 2:
        fmt = GL_RG;
        break;
    case 3:
        fmt = GL_RGB;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, _glID);

    DuskLogVerbose("Binding texture to ID %u", _glID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.WrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.MinFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)fmt, _size.x, _size.y, 0, fmt, GL_UNSIGNED_BYTE, buffer);

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
