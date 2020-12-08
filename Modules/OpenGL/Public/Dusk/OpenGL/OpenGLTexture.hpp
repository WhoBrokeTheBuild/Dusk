#ifndef DUSK_OPENGL_TEXTURE_HPP
#define DUSK_OPENGL_TEXTURE_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Texture.hpp>
#include <Dusk/TextureImporter.hpp>

namespace Dusk {

#define DUSK_OPENGL_TEXTURE(x) (dynamic_cast<Dusk::OpenGLTexture *>(x))

class DUSK_OPENGL_API OpenGLTexture : public Texture 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLTexture)

    OpenGLTexture() = default;

    virtual ~OpenGLTexture();

    inline std::string GetClassID() const override {
        return "Dusk::OpenGLTexture";
    }

    bool Load(const TextureData * data, Options opts = Options()) override;

    void Bind();

private:

    GLenum GetGLDataFormat(int components);
    
    GLenum GetGLDataType(const TextureData::DataType& type);

    GLenum GetGLWrapType(const WrapType& type);

    GLenum GetGLFilterType(const FilterType& type);

    GLuint _glID = 0;

}; // class OpenGLTexture

} // namespace Dusk

#endif // DUSK_OPENGL_TEXTURE_HPP
