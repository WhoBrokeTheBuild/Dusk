#ifndef DUSK_OPENGL_TEXTURE_HPP
#define DUSK_OPENGL_TEXTURE_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API Texture : public Dusk::Texture 
{
public:

    virtual ~Texture();

    bool Load(const TextureData * data) override;

    void Bind() override;

private:

    GLenum GetGLDataFormat(int components);
    GLenum GetGLDataType(const TextureData::DataType& type);
    GLenum GetGLWrapType(const TextureData::WrapType& type);
    GLenum GetGLFilterType(const TextureData::FilterType& type);

    GLuint _glID = 0;

}; // class Texture

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_TEXTURE_HPP
