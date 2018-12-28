#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <dusk/Config.hpp>

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <algorithm>
using std::swap;

namespace dusk {

class Texture
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Texture)

    Texture() = default;
    Texture(const string& filename)
    {
        LoadFromFile(filename);
    }

    Texture(const glm::uvec2& size, const vector<uint8_t>& pixels, GLenum type = GL_RGBA)
    {
        LoadFromData(size, pixels, type);
    }

    Texture(Texture&& rhs)
    {
        swap(_loaded, rhs._loaded);
        swap(_glID, rhs._glID);
    }

    virtual ~Texture()
    {
        if (_glID > 0) glDeleteTextures(1, &_glID);
    }

    /// Methods

    bool LoadFromFile(const string& filename);

    bool LoadFromData(const glm::uvec2& size, const vector<uint8_t>& pixels, GLenum type = GL_RGBA);

    void Bind();

    bool IsLoaded() const { return _loaded; }

private:

    bool FinishLoad(const glm::uvec2& size, const vector<uint8_t>& pixels, GLenum type);

    bool _loaded = false;

    GLuint _glID = 0;

}; // class Texture

} // namespace dusk

#endif // DUSK_TEXTURE_HPP
