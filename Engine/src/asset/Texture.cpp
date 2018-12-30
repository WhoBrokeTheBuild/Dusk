#include "dusk/asset/Texture.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

#include <stb/stb_image.h>
#include <algorithm>

namespace dusk {

Texture::Texture(const std::string& filename)
{
    LoadFromFile(filename);
}

Texture::Texture(const std::string& filename, Options opts)
{
    LoadFromFile(filename, opts);
}

Texture::Texture(GLuint&& id, glm::ivec2 size)
{
    std::swap(_glID, id);
    _size = size;
    if (_glID > 0) 
    {
        _loaded = true;
    }
}

Texture::Texture(Texture&& rhs)
{
    std::swap(_loaded, rhs._loaded);
    std::swap(_glID, rhs._glID);
}

Texture::~Texture() 
{
    if (_glID > 0) {
        glDeleteTextures(1, &_glID);
    }
}

bool Texture::LoadFromFile(const std::string& filename)
{
    return LoadFromFile(filename, Options{});
}

bool Texture::LoadFromFile(const std::string& filename, Options opts)
{
    DuskBenchStart();

    _loaded = false;

    if (_glID > 0)
    {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    const auto& paths = GetAssetPaths();

    int comp;
    glm::ivec2 size;
    unsigned char * texture = nullptr;

    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        texture = stbi_load(fullPath.c_str(), &_size.x, &_size.y, &comp, STBI_rgb_alpha);

        if (texture) break;
    }

    if (!texture)
    {
        DuskLogError("Failed to load texture '%s'", filename.c_str());
        return false;
    }

    glGenTextures(1, &_glID);

    if (0 == _glID)
    {
        DuskLogError("Failed to create GL Texture");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, _glID);
    DuskLogVerbose("Binding texture to ID %u", _glID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, opts.WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, opts.WrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, opts.MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, opts.MinFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

    if (opts.Mipmap) 
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texture);

    DuskBenchEnd("Texture::LoadFromFile");
    return _loaded;
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _glID);
}

} // namespace dusk
