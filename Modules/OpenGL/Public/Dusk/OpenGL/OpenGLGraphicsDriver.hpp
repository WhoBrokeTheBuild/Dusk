#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/SDL2/SDL2GraphicsDriver.hpp>
#include <Dusk/OpenGL/OpenGLPipeline.hpp>
#include <Dusk/OpenGL/OpenGLTexture.hpp>
#include <Dusk/OpenGL/OpenGLShader.hpp>
#include <Dusk/OpenGL/OpenGLMesh.hpp>

namespace Dusk::OpenGL {

struct ShaderDataBindingInfo
{
    GLuint glID;
    size_t size;
};

class DUSK_OPENGL_API OpenGLGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLGraphicsDriver)

    OpenGLGraphicsDriver() = default;

    virtual ~OpenGLGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void SwapBuffers() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;

private:

    SDL_GLContext _glContext = nullptr;

    std::unordered_map<std::string, GLuint> _shaderDataBindings;

    std::vector<std::weak_ptr<Shader>> _shaders;
    
}; // class OpenGLGraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
