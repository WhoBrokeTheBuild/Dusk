#include "dusk/asset/Texture.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

bool Texture::LoadFromFile(const std::string& filename)
{
    DuskBenchStart();

    const auto& paths = GetAssetPaths();

    int width, height, comp;
    unsigned char * texture = nullptr;

    std::string fullPath;
    for (auto& p : paths) {
        fullPath = p + filename;

        DuskLogVerbose("Checking %s", fullPath.c_str());
        texture = stbi_load(fullPath.c_str(), &width, &height, &comp, STBI_rgb_alpha);

        if (texture) break;
    }

    if (!texture)
    {
        DuskLogError("Failed to load texture '%s'", filename.c_str());
        return false;
    }

    std::vector<uint8_t> data(texture, texture + (width * height * GetGLTypeSize(GL_RGBA)));
    stbi_image_free(texture);

    DuskLogLoad("Finished loading texture from '%s'", fullPath.c_str());

    bool success = FinishLoad(glm::uvec2(width, height), data, GL_RGBA);
    DuskBenchEnd("Texture::LoadFromFile");
    return success;
}

bool Texture::LoadFromData(const glm::uvec2& size, const std::vector<uint8_t>& pixels, GLenum type /*= GL_RGBA*/)
{
    DuskBenchStart();
    DuskLogLoad("Loading texture from data");

    bool success = FinishLoad(size, pixels, type);
    DuskBenchEnd("Texture::LoadFromFile");
    return success;
}

bool Texture::FinishLoad(const glm::uvec2& size, const std::vector<uint8_t>& data, GLenum type)
{
    if (_glID > 0)
    {
        glDeleteTextures(1, &_glID);
        _glID = 0;
    }

    _loaded = false;

    if (data.size() < (size.x * size.y * GetGLTypeSize(type)))
    {
        DuskLogError("Image size mismatch, buffer to small");
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Sharper
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Smoother
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, type, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    _loaded = true;
    return true;
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _glID);
}

} // namespace dusk
