#ifndef DUSK_OPENGL_TEXTURE_HPP
#define DUSK_OPENGL_TEXTURE_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Texture.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API Texture : public ITexture 
{
public:

    virtual ~Texture();

    bool Load(const ITextureData * data) override;

    void Bind() override;

private:

    GLenum GetGLDataFormat(int components);
    GLenum GetGLDataType(const ITextureData::DataType& type);
    GLenum GetGLWrapType(const ITextureData::WrapType& type);
    GLenum GetGLFilterType(const ITextureData::FilterType& type);

    GLuint _glID = 0;

}; // class Texture

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_TEXTURE_HPP
