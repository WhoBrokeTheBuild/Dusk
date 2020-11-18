#ifndef DUSK_OPENGL_TEXTURE_HPP
#define DUSK_OPENGL_TEXTURE_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>
#include <Dusk/Graphics/TextureImporter.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API Texture : public Dusk::Texture 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Texture)

    explicit Texture() = default;

    virtual ~Texture();

    bool Load(const TextureData * data, Options opts = Options()) override;

    void Bind() override;

private:

    GLenum GetGLDataFormat(int components);
    GLenum GetGLDataType(const TextureData::DataType& type);
    GLenum GetGLWrapType(const WrapType& type);
    GLenum GetGLFilterType(const FilterType& type);

    GLuint _glID = 0;

}; // class Texture

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_TEXTURE_HPP
