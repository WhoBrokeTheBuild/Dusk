#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/OpenGL/InputDriver.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/OpenGL/Texture.hpp>
#include <Dusk/OpenGL/Shader.hpp>
#include <Dusk/OpenGL/Mesh.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

struct ShaderDataBindingInfo
{
    GLuint glID;
    size_t size;
};

class DUSK_OPENGL_API GraphicsDriver : public Dusk::GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(GraphicsDriver)

    GraphicsDriver();

    virtual ~GraphicsDriver();

    inline std::string GetClassID() const override {
        return "Dusk::OpenGL::GraphicsDriver";
    }

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::shared_ptr<Dusk::Texture> CreateTexture() override;

    std::shared_ptr<Dusk::Shader> CreateShader() override;

    std::shared_ptr<Dusk::Mesh> CreateMesh() override;

    bool SetShaderData(const std::string& name, size_t size, void * data) override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL_GLContext _glContext;

    InputDriver * _inputDriver = nullptr;

    std::unordered_map<std::string, GLuint> _shaderDataBindings;

    std::vector<std::weak_ptr<Shader>> _shaders;
    
}; // class GraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
