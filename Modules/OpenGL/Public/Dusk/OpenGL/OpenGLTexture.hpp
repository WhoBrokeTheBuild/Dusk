#ifndef DUSK_OPENGL_TEXTURE_HPP
#define DUSK_OPENGL_TEXTURE_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Texture.hpp>
#include <Dusk/TextureImporter.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_TEXTURE(x) (dynamic_cast<Dusk::OpenGL::OpenGLTexture *>(x))

class DUSK_OPENGL_API OpenGLTexture : public Texture 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLTexture)

    OpenGLTexture() = default;

    virtual ~OpenGLTexture();

    bool Load(const std::unique_ptr<TextureData>& data, Options opts = Options()) override;

    inline GLuint GetGLID() {
        return _glID;
    }

private:

    GLenum GetGLDataType(const TextureDataType& type);

    GLenum GetGLWrapType(const TextureWrapType& type);

    GLenum GetGLFilterType(const TextureFilterType& type);

    GLuint _glID = 0;

}; // class OpenGLTexture

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_TEXTURE_HPP
