#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/OpenGL/OpenGLInputDriver.hpp>
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

class DUSK_OPENGL_API OpenGLGraphicsDriver : public GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLGraphicsDriver)

    OpenGLGraphicsDriver() = default;

    virtual ~OpenGLGraphicsDriver() = default;

    inline std::string GetClassID() const override {
        return "Dusk::GraphicsDriver";
    }

    bool Initialize() override;

    void Terminate() override;

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL_GLContext _glContext = nullptr;

    OpenGLInputDriver * _inputDriver = nullptr;

    std::unordered_map<std::string, GLuint> _shaderDataBindings;

    std::vector<std::weak_ptr<Shader>> _shaders;
    
}; // class OpenGLGraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
