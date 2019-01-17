#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <dusk/asset/ILoadable.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/OpenGL.hpp>

#include <string>
#include <memory>

namespace dusk {

class Texture : public ILoadable
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

    Texture(const std::string&, Options = Options());

    Texture(const uint8_t *, glm::ivec2, int comp = 4, Options = Options());

    Texture(GLuint&&, glm::ivec2);

    Texture(Texture&&);

    virtual ~Texture();

    /// Methods

    bool LoadFromFile(const std::string&, Options = Options());

    bool LoadFromBuffer(const uint8_t *, glm::ivec2, int comp = 4, Options = Options());

    void Bind();

    GLuint GetGLID() const { 
        return _glID; 
    }

    glm::ivec2 GetSize() const { 
        return _size; 
    }

private:

    GLuint _glID = 0;

    glm::ivec2 _size;

}; // class Texture

} // namespace dusk

#endif // DUSK_TEXTURE_HPP
