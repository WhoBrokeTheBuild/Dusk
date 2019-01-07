#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/OpenGL.hpp>

#include <string>
#include <memory>

namespace dusk {

class Texture
{
public:

    struct Options 
    {
        Options()
            : WrapS(GL_REPEAT)
            , WrapT(GL_REPEAT)
            , MagFilter(GL_NEAREST)
            , MinFilter(GL_NEAREST)
            , Mipmap(true)
        { }

        GLenum WrapS;
        GLenum WrapT;

        GLenum MagFilter;
        GLenum MinFilter;

        bool Mipmap;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Texture)

    Texture() = default;

    Texture(const std::string& filename, Options opts = Options());

    Texture(const uint8_t * buffer, glm::ivec2 size, int comp = 4, Options opts = Options());

    Texture(GLuint&& id, glm::ivec2 size);

    Texture(Texture&& rhs);

    virtual ~Texture();

    /// Methods

    bool LoadFromFile(const std::string& filename, Options opts = Options());

    bool LoadFromBuffer(const uint8_t * buffer, glm::ivec2 size, int comp = 4, Options opts = Options());

    void Bind();

    bool IsLoaded() const { 
        return _loaded; 
    }

    GLuint GetGLID() const { 
        return _glID; 
    }

    glm::ivec2 GetSize() const { 
        return _size; 
    }

private:

    bool _loaded = false;

    GLuint _glID = 0;

    glm::ivec2 _size;

}; // class Texture

} // namespace dusk

#endif // DUSK_TEXTURE_HPP
