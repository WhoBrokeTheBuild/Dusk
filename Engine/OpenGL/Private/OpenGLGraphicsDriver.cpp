#include <Dusk/Drivers/OpenGLGraphicsDriver.hpp>

#include <Dusk/Dusk.hpp>

#include <glad/glad.h>

#undef CreateWindow
#undef min
#undef max
#undef near
#undef far

namespace Dusk::OpenGL {

GraphicsDriver::GraphicsDriver() {

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    _sdlWindow = SDL_CreateWindow("Dusk", 
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
        fprintf(stderr, "Failed to create OpenGL context, %s\n", SDL_GetError());
        return;
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        return;
    }
    
    printf("OpenGL Version %s\n", glGetString(GL_VERSION));
    printf("GLSL Version %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("OpenGL Vendor %s\n", glGetString(GL_VENDOR));
    printf("OpenGL Renderer %s\n", glGetString(GL_RENDERER));
}

GraphicsDriver::~GraphicsDriver() {
    SDL_DestroyWindow(_sdlWindow);
}

void GraphicsDriver::SetTitle(const std::string& title) {
    SDL_SetWindowTitle(_sdlWindow, title.c_str());
}

std::string GraphicsDriver::GetTitle() {
    return SDL_GetWindowTitle(_sdlWindow);
}

void GraphicsDriver::SetSize(const ivec2& size) {
    SDL_SetWindowSize(_sdlWindow, size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}

ivec2 GraphicsDriver::GetSize() {
    ivec2 size;
    SDL_GetWindowSize(_sdlWindow, &size.x, &size.y);
    return size;
}

void GraphicsDriver::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) {
            SetRunning(false);
        }
    }
}

void GraphicsDriver::SwapBuffers() {
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(_sdlWindow);
}

} // namespace Dusk::OpenGL