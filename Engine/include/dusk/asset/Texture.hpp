#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <dusk/Config.hpp>

#include <string>

namespace dusk {

class Texture
{
public:

    struct Options 
    {
		Options() = default;

        GLenum WrapS = GL_REPEAT;
        GLenum WrapT = GL_REPEAT;

        GLenum MagFilter = GL_NEAREST;
        GLenum MinFilter = GL_NEAREST;

        bool Mipmap = true;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Texture)

    Texture() = default;

    Texture(const std::string& filename, Options opts = Options());

	Texture(const uint8_t * data, glm::ivec2 size, int comp = 4, Options opts = Options());

    Texture(GLuint&& id, glm::ivec2 size);

    Texture(Texture&& rhs);

    virtual ~Texture();

    /// Methods

    bool LoadFromFile(const std::string& filename, Options opts = Options());

	bool LoadFromBuffer(const uint8_t * data, glm::ivec2 size, int comp = 4, Options opts = Options());

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
