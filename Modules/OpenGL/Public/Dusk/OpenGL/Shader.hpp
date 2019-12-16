#ifndef DUSK_OPENGL_SHADER_HPP
#define DUSK_OPENGL_SHADER_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Shader.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API Shader : public IShader 
{
public:

    bool LoadFromFiles(const std::vector<std::string>& filenames) override;

    void Bind() override;

private:

    GLuint LoadSPV(const std::string& filename);
    GLuint LoadGLSL(const std::string& filename);

    GLenum GetGLShaderType(const std::string& filename);

    GLuint _glID = 0;

}; // class Shader

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_SHADER_HPP