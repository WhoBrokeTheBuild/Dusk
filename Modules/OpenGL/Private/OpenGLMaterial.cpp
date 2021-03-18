#include <Dusk/OpenGL/OpenGLMaterial.hpp>
#include <Dusk/OpenGL/OpenGLBuffer.hpp>
#include <Dusk/OpenGL/OpenGLTexture.hpp>
#include <Dusk/GraphicsDriver.hpp>

namespace Dusk::OpenGL {

void OpenGLMaterial::Bind()
{
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    OpenGLBuffer * glMaterialBuffer = static_cast<OpenGLBuffer *>(_shaderMaterialBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, DUSK_SHADER_MATERIAL_BINDING, glMaterialBuffer->GetGLID());

    OpenGLTexture * glDefaultTexture = static_cast<OpenGLTexture *>(gfx->GetDefaultTexture().get());
    GLuint defaultTextureID = (glDefaultTexture ? glDefaultTexture->GetGLID() : 0);

    OpenGLTexture * glTexture = nullptr;

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::BaseColorMap));
    glTexture = DUSK_OPENGL_TEXTURE(_baseColorMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::NormalMap));
    glTexture = DUSK_OPENGL_TEXTURE(_normalMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::MetallicRoughnessMap));
    glTexture = DUSK_OPENGL_TEXTURE(_metallicRoughnessMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::EmissiveMap));
    glTexture = DUSK_OPENGL_TEXTURE(_emissiveMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::OcclusionMap));
    glTexture = DUSK_OPENGL_TEXTURE(_occlusionMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : defaultTextureID));
}

} // namespace Dusk::OpenGL