#include <Dusk/OpenGL/GraphicsDriver.hpp>
#include <Dusk/OpenGL/Texture.hpp>
#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>

namespace Dusk::OpenGL {

DUSK_OPENGL_API
GraphicsDriver::GraphicsDriver() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DuskLogError("Failed to initialize SDL, %s", SDL_GetError());
        return;
    }

    SDL_version version;
    SDL_GetVersion(&version);
    DuskLogVerbose("SDL Version: %d.%d.%d", (int)version.major, (int)version.minor, (int)version.patch);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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

    Uint16 pixels[16 * 16] = { 0xFFFF };
    SDL_Surface * surface = SDL_CreateRGBSurfaceFrom(pixels, 16, 16, 16, 16 * 2,
                                                     0x0f00, 0x00f0, 0x000f, 0xf000);
    SDL_SetWindowIcon(_sdlWindow, surface);
    SDL_FreeSurface(surface);

    _sdlContext = SDL_GL_CreateContext(_sdlWindow);
    if (!_sdlContext) {
        DuskLogError("Failed to create OpenGL context, %s", SDL_GetError());
        return;
    }

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        DuskLogError("Failed to initialize OpenGL context");
        return;
    }
    
    DuskLogVerbose("OpenGL Version: %s",  glGetString(GL_VERSION));
    DuskLogVerbose("GLSL Version: %s",    glGetString(GL_SHADING_LANGUAGE_VERSION));
    DuskLogVerbose("OpenGL Vendor: %s",   glGetString(GL_VENDOR));
    DuskLogVerbose("OpenGL Renderer: %s", glGetString(GL_RENDERER));
}

DUSK_OPENGL_API
GraphicsDriver::~GraphicsDriver() {
    SDL_GL_DeleteContext(_sdlContext);
    SDL_DestroyWindow(_sdlWindow);
    SDL_Quit();
}

DUSK_OPENGL_API
void GraphicsDriver::SetWindowTitle(const std::string& title) {
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

DUSK_OPENGL_API
std::string GraphicsDriver::GetWindowTitle() {
    return SDL_GetWindowTitle(_sdlWindow);
}

DUSK_OPENGL_API
void GraphicsDriver::SetWindowSize(const ivec2& size) {
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}

DUSK_OPENGL_API
ivec2 GraphicsDriver::GetWindowSize() {
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

DUSK_OPENGL_API
void GraphicsDriver::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) {
            SetRunning(false);
        }
    }
}

DUSK_OPENGL_API
void GraphicsDriver::SwapBuffers() {
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(_sdlWindow);
}

std::unique_ptr<ITexture> GraphicsDriver::CreateTexture()
{
    return std::make_unique<Texture>();
}

std::unique_ptr<IShader> GraphicsDriver::CreateShader()
{
    return nullptr;
}

} // namespace Dusk::OpenGL
