#ifndef DUSK_OPENGL_SHADER_HPP
#define DUSK_OPENGL_SHADER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Shader.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_SHADER(x) (dynamic_cast<Dusk::OpenGL::OpenGLShader *>(x))

class DUSK_OPENGL_API OpenGLShader : public Shader 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLShader)

    OpenGLShader() = default;

    bool LoadFromFiles(const std::vector<std::string>& filenames, bool useAssetPath = true) override;

    void Bind();

    GLuint GetID();

private:

    GLuint LoadSPV(const std::string& filename, bool useAssetPath);
    
    GLuint LoadGLSL(const std::string& filename, bool useAssetPath);

    GLenum GetGLShaderType(const std::string& filename);

    GLuint _glID = 0;

}; // class OpenGLShader

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_SHADER_HPP