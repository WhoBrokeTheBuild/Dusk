#ifndef DUSK_OPENGL_GRAPHICS_DRIVER_HPP
#define DUSK_OPENGL_GRAPHICS_DRIVER_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/SDL2/SDL2GraphicsDriver.hpp>
#include <Dusk/OpenGL/OpenGLPipeline.hpp>
#include <Dusk/OpenGL/OpenGLTexture.hpp>
#include <Dusk/OpenGL/OpenGLShader.hpp>
#include <Dusk/OpenGL/OpenGLMesh.hpp>
#include <Dusk/OpenGL/OpenGLPrimitive.hpp>
#include <Dusk/OpenGL/OpenGLBuffer.hpp>

namespace Dusk::OpenGL {

struct ShaderDataBindingInfo
{
    GLuint glID;
    size_t size;
};

#define DUSK_OPENGL_GRAPHICS_DRIVER(x) (dynamic_cast<Dusk::OpenGL::OpenGLGraphicsDriver *>(x))

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

    std::unique_ptr<Primitive> CreatePrimitive() override;

    // bool SetShaderData(const std::string& name, size_t size, void * data) override;






    

    bool AddConstantBuffer(std::shared_ptr<Buffer> buffer, unsigned binding);

    bool RemoveConstantBuffer(unsigned binding);

    std::shared_ptr<Buffer> GetConstantBuffer(unsigned binding);

    void TermConstantBuffers();

private:

    void BindUniformBufferObjects();

    void InitDebugMessageCallback();

    SDL_GLContext _glContext = nullptr;

    std::unordered_map<unsigned, std::shared_ptr<Buffer>> _constantBufferBindings;

    std::vector<std::weak_ptr<Shader>> _shaders;
    
}; // class OpenGLGraphicsDriver

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_GRAPHICS_DRIVER_HPP
