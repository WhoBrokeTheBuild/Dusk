#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/SDL2/SDL2GraphicsDriver.hpp>
#include <Dusk/OpenGL/OpenGLPipeline.hpp>
#include <Dusk/OpenGL/OpenGLTexture.hpp>
#include <Dusk/OpenGL/OpenGLShader.hpp>
#include <Dusk/OpenGL/OpenGLMesh.hpp>
#include <Dusk/OpenGL/OpenGLMaterial.hpp>
#include <Dusk/OpenGL/OpenGLPrimitive.hpp>
#include <Dusk/OpenGL/OpenGLBuffer.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API OpenGLGraphicsDriver : public SDL2::SDL2GraphicsDriver
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLGraphicsDriver)

    OpenGLGraphicsDriver() = default;

    virtual ~OpenGLGraphicsDriver() = default;

    bool Initialize() override;

    void Terminate() override;

    void Render() override;

    std::shared_ptr<Buffer> CreateBuffer() override;

    std::shared_ptr<Pipeline> CreatePipeline(std::shared_ptr<Shader> shader) override;

    std::shared_ptr<Texture> CreateTexture() override;

    std::shared_ptr<Shader> CreateShader() override;

    std::shared_ptr<Mesh> CreateMesh() override;

    std::shared_ptr<Material> CreateMaterial() override;

    std::shared_ptr<Primitive> CreatePrimitive() override;

private:

    void BindUniformBufferObjects();

    void RegisterDebugMessageCallback();

    SDL_GLContext _glContext = nullptr;

    std::vector<std::weak_ptr<Shader>> _shaderList;

}; // class OpenGLGraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
