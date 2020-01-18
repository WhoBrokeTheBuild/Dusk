#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>

#include <SDL.h>

namespace Dusk::OpenGL {

struct ShaderDataBindingInfo
{
    GLuint glID;
    size_t size;
};

class DUSK_OPENGL_API GraphicsDriver : public IGraphicsDriver
{
public:

    GraphicsDriver();

    virtual ~GraphicsDriver();

    void SetWindowTitle(const std::string& title) override;

    std::string GetWindowTitle() override;

    void SetWindowSize(const ivec2& size) override;

    ivec2 GetWindowSize() override;

    void ProcessEvents() override;

    void SwapBuffers() override;

    std::unique_ptr<ITexture> CreateTexture() override;

    std::unique_ptr<IShader> CreateShader() override;

    std::unique_ptr<IMesh> CreateMesh() override;

    bool SetShaderData(unsigned binding, size_t size, void * data) override;

private:

    SDL_Window * _sdlWindow = nullptr;

    SDL_GLContext _glContext;

    std::unordered_map<unsigned, GLuint> _shaderDataBindings;
    
}; // class GraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
