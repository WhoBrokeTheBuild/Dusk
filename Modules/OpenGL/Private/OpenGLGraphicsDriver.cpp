#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
bool OpenGLGraphicsDriver::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        DuskLogError("Failed to initialize SDL, %s", SDL_GetError());
        return false;
    }

    SDL_version version;
    SDL_GetVersion(&version);
    DuskLogVerbose("SDL Version: %d.%d.%d", (int)version.major, (int)version.minor, (int)version.patch);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    std::string title = GetApplicationName() + " (" + GetApplicationVersion().GetString() + ")";

    _sdlWindow = SDL_CreateWindow(title.c_str(), 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480, 
        SDL_WINDOW_OPENGL);

    if (!_sdlWindow) {
        DuskLogError("SDL_CreateWindow() failed, %s", SDL_GetError());
        return false;
    }

    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
                                                     0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);

    _glContext = SDL_GL_CreateContext(_sdlWindow);
    if (!_glContext) {
        DuskLogError("Failed to create OpenGL context, %s", SDL_GetError());
        return false;
    }

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        DuskLogError("Failed to initialize OpenGL context");
        return false;
    }
    
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

    glClearColor(0.392f, 0.584f, 0.929f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ONE_MINUS_SRC_ALPHA);

    _inputDriver = New OpenGLInputDriver();
    SetInputDriver(std::unique_ptr<InputDriver>(_inputDriver));

    return true;
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::Terminate()
{
    _inputDriver = nullptr;
    SetInputDriver(nullptr);

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_sdlWindow);
    
    SDL_Quit();
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::SetWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

DUSK_OPENGL_API
std::string OpenGLGraphicsDriver::GetWindowTitle()
{
    return SDL_GetWindowTitle(_sdlWindow);
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::SetWindowSize(const ivec2& size)
{
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}

DUSK_OPENGL_API
ivec2 OpenGLGraphicsDriver::GetWindowSize()
{
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            SetRunning(false);
        }

        _inputDriver->ProcessEvent(&event);

        if (event.type == SDL_WINDOWEVENT)
        {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            {
                WindowResizedEventData data;
                data.Size = { event.window.data1, event.window.data2 };
                WindowResizedEvent.Call(&data);

                glViewport(0, 0, event.window.data1, event.window.data2);
                break;
            }
            }
        }
    }
    
    // TODO: Move
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

DUSK_OPENGL_API
void OpenGLGraphicsDriver::SwapBuffers()
{
    SDL_GL_SwapWindow(_sdlWindow);
}

DUSK_OPENGL_API
std::shared_ptr<Pipeline> OpenGLGraphicsDriver::CreatePipeline()
{
    return std::shared_ptr<Pipeline>(New OpenGLPipeline());
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
    _shaders.push_back(shader);
    return shader;
}

DUSK_OPENGL_API
std::shared_ptr<Mesh> OpenGLGraphicsDriver::CreateMesh()
{
    return std::shared_ptr<Mesh>(New OpenGLMesh());
}

DUSK_OPENGL_API
bool OpenGLGraphicsDriver::SetShaderData(const std::string& name, size_t size, void * buffer)
{
    GLuint glID = 0;

    const auto& it = _shaderDataBindings.find(name);
    if (it == _shaderDataBindings.cend()) {
        DuskLogVerbose("Querying uniform block binding for '%s'", name);

        for (int i = 0; i < _shaders.size(); ++i) {
            if (auto shader = _shaders[i].lock()) {
                OpenGLShader * glShader = DUSK_OPENGL_SHADER(shader.get());
                GLuint index = glGetUniformBlockIndex(glShader->GetID(), name.c_str());
                if (index == GL_INVALID_OPERATION || index == GL_INVALID_INDEX) {
                    continue;
                }

                DuskLogVerbose("Uniform block '%s' is at index %d", name, index);

                glGenBuffers(1, &glID);
                glBindBuffer(GL_UNIFORM_BUFFER, glID);
                glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                glBindBufferBase(GL_UNIFORM_BUFFER, index, glID);

                _shaderDataBindings.emplace(name, glID);
                break;
            }
            else {
                _shaders.erase(_shaders.begin() + i);
                --i;
            }
        }
        if (glID == 0) {
            return false;
        }
    }
    else {
        glID = it->second;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, glID);

    GLvoid * ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, buffer, size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return true;
}

} // namespace Dusk::OpenGL
