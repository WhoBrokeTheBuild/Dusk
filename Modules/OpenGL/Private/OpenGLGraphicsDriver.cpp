#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Scene.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLGraphicsDriver::Initialize()
{
    if (!SDL2GraphicsDriver::Initialize()) {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    #if defined(DUSK_BUILD_DEBUG)

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    #endif

    if (!SDL2GraphicsDriver::CreateWindow(SDL_WINDOW_OPENGL)) {
        return false;
    }

    _glContext = SDL_GL_CreateContext(GetSDL2Window());
    if (!_glContext) {
        DuskLogError("Failed to create OpenGL context, %s", SDL_GetError());
        return false;
    }

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        DuskLogError("Failed to initialize OpenGL context");
        return false;
    }

    #if defined(DUSK_BUILD_DEBUG)

        InitDebugMessageCallback();

    #endif

    SDL_GL_SetSwapInterval(0);
    
    DuskLogVerbose("OpenGL Version: %s",  glGetString(GL_VERSION));
    DuskLogVerbose("GLSL Version: %s",    glGetString(GL_SHADING_LANGUAGE_VERSION));
    DuskLogVerbose("OpenGL Vendor: %s",   glGetString(GL_VENDOR));
    DuskLogVerbose("OpenGL Renderer: %s", glGetString(GL_RENDERER));

    int value = 0;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
    DuskLogVerbose("Max Vertex Attributes: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &value);
    DuskLogVerbose("Max Vertex Uniform Components: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &value);
    DuskLogVerbose("Max Vertex Output Components: %d", value);

    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
    DuskLogVerbose("Max Fragment Uniform Components: %d", value);

    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &value);
    DuskLogVerbose("Max Fragment Input Components: %d", value);

    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &value);
    DuskLogVerbose("Max Draw Buffers: %d", value);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
    DuskLogVerbose("Max Fragment Texture Image Units: %d", value);

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &value);
    DuskLogVerbose("Max Vertex Texture Image Units: %d", value);
    
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
    DuskLogVerbose("Max Uniform Buffer Bindings: %d", value);
    
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &value);
    DuskLogVerbose("Max Vertex Uniform Blocks: %d", value);
    
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &value);
    DuskLogVerbose("Max Fragment Uniform Blocks: %d", value);
    
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
    DuskLogVerbose("Max Uniform Block Size: %d", value);

    InitializeUpdateContext();
    InitializeRenderContext();
    InitializeConstantBuffers();

    return true;
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::Terminate()
{
    if (_glContext) {
        SDL_GL_DeleteContext(_glContext);
        _glContext = nullptr;
    }

    SDL2GraphicsDriver::Terminate();
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::Render()
{
    vec4 cc = GetClearColor();
    glClearColor(cc[0], cc[1], cc[2], cc[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GraphicsDriver::Render();

    uint8_t * data = reinterpret_cast<uint8_t *>(_renderContext->GetShaderGlobals());
    _shaderGlobalsBuffer->WriteTo(0, sizeof(ShaderGlobals), data);

    OpenGLBuffer * buffer = DUSK_OPENGL_BUFFER(_shaderGlobalsBuffer.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, DUSK_SHADER_GLOBALS_BINDING, buffer->GetGLID());

    Scene * scene = GetCurrentScene();
    if (scene) {
        scene->Render(_renderContext.get());
    }

    SDL_GL_SwapWindow(GetSDL2Window());
}

DUSK_OPENGL_API
std::shared_ptr<Buffer> OpenGLGraphicsDriver::CreateBuffer()
{
    return std::shared_ptr<Buffer>(New OpenGLBuffer());
}

DUSK_OPENGL_API
std::shared_ptr<Pipeline> OpenGLGraphicsDriver::CreatePipeline(std::shared_ptr<Shader> shader)
{
    auto ptr = std::shared_ptr<Pipeline>(New OpenGLPipeline());
    ptr->SetShader(shader);
    return ptr;
}

DUSK_OPENGL_API
std::shared_ptr<Texture> OpenGLGraphicsDriver::CreateTexture()
{
    return std::shared_ptr<Texture>(New OpenGLTexture());
}

DUSK_OPENGL_API
std::shared_ptr<Shader> OpenGLGraphicsDriver::CreateShader()
{
    auto shader = std::shared_ptr<Shader>(New OpenGLShader());
    _shaderList.push_back(shader);
    return shader;
}

DUSK_OPENGL_API
std::shared_ptr<Mesh> OpenGLGraphicsDriver::CreateMesh()
{
    auto ptr = std::shared_ptr<Mesh>(New OpenGLMesh());
    ptr->Initialize();
    return ptr;
}

DUSK_OPENGL_API
std::shared_ptr<Primitive> OpenGLGraphicsDriver::CreatePrimitive()
{
    return std::shared_ptr<Primitive>(New OpenGLPrimitive());
}

void GLAPIENTRY
_OpenGLDebugMessageCallback(
    GLenum source, GLenum type, GLenum id, GLenum severity,
    GLsizei length, const GLchar * message, const void * userData)
{
    if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        Log(LogLevel::Performance, "[PERF](OpenGLDebugMessage) %s\n", message);
    }
    else {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log(LogLevel::Error, "[ERRO](OpenGLDebugMessage) %s\n", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log(LogLevel::Warning, "[WARN](OpenGLDebugMessage) %s\n", message);
            break;

        #if defined(DUSK_ENABLE_VERBOSE_LOGGING)

            case GL_DEBUG_SEVERITY_NOTIFICATION:
                Log(LogLevel::Verbose, "[VERB](OpenGLDebugMessage) %s\n", message);
                break;
                
        #endif

        }
    }
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::InitDebugMessageCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(_OpenGLDebugMessageCallback, nullptr);
}

} // namespace Dusk::OpenGL
