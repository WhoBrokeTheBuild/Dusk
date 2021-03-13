#include <Dusk/OpenGL/OpenGLMaterial.hpp>
#include <Dusk/OpenGL/OpenGLBuffer.hpp>
#include <Dusk/OpenGL/OpenGLTexture.hpp>

namespace Dusk::OpenGL {

void OpenGLMaterial::Bind()
{
    OpenGLBuffer * glMaterialBuffer = DUSK_OPENGL_BUFFER(_shaderMaterialBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, DUSK_SHADER_MATERIAL_BINDING, glMaterialBuffer->GetGLID());

    OpenGLTexture * glTexture = nullptr;

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::BaseColorMap));
    glTexture = DUSK_OPENGL_TEXTURE(_baseColorMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : 0));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::NormalMap));
    glTexture = DUSK_OPENGL_TEXTURE(_normalMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : 0));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::MetallicRoughnessMap));
    glTexture = DUSK_OPENGL_TEXTURE(_metallicRoughnessMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : 0));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::EmissiveMap));
    glTexture = DUSK_OPENGL_TEXTURE(_emissiveMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : 0));

    glActiveTexture(GL_TEXTURE0 + GetMaterialTextureBinding(MaterialTexture::OcclusionMap));
    glTexture = DUSK_OPENGL_TEXTURE(_occlusionMap.get());
    glBindTexture(GL_TEXTURE_2D, (glTexture ? glTexture->GetGLID() : 0));
}

} // namespace Dusk::OpenGL