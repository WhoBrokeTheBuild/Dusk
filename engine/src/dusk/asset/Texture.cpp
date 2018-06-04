#include "dusk/asset/Texture.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

Texture::Texture(const std::string& filename)
    : Asset(filename)
{ }

Texture::~Texture()
{
    Free();
}

void Texture::Load()
{
    DuskBenchStart();

    const auto& paths = GetAssetPaths();

    int width, height, comp;
    unsigned char * texture = nullptr;

    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + GetFilename();

        DuskLogVerbose("Checking %s", fullPath.c_str());
        texture = stbi_load(fullPath.c_str(), &width, &height, &comp, STBI_rgb_alpha);

        if (texture) break;
    }

    if (!texture)
    {
        DuskLogError("Failed to load texture '%s'", GetFilename().c_str());
        SetFilename("");
    }

    std::vector<uint8_t> data(texture, texture + (width * height * GetGLTypeSize(GL_RGBA)));
    stbi_image_free(texture);

    DuskLogLoad("Finished loading texture from '%s'", fullPath.c_str());

    if (_glID > 0)
    {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    //if (data.size() < (width * height * GetGLTypeSize(GL_RGBA)))
    //{
    //    DuskLogError("Image size mismatch, buffer to small");
    //    SetFilename("");
    //}

    glGenTextures(1, &_glID);

    if (0 == _glID)
    {
        DuskLogError("Failed to create GL Texture");
        SetFilename("");
    }

    glBindTexture(GL_TEXTURE_2D, _glID);
    DuskLogVerbose("Binding texture to ID %u", _glID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Sharper
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Smoother
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    SetLoaded(true);
    DuskBenchEnd("Texture::Load");
}

void Texture::Free()
{
    if (_glID > 0) {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }
}

void Texture::Bind()
{
    if (!IsLoaded()) {
        Load();
    }

    glBindTexture(GL_TEXTURE_2D, _glID);
}

} // namespace dusk
