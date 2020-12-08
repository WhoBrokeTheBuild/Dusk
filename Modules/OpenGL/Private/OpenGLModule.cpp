#include <Dusk/Module.hpp>

#include <Dusk/OpenGL/OpenGLGraphicsDriver.hpp>

using namespace Dusk;

void OpenGLModuleInit()
{
    SetGraphicsDriver(std::unique_ptr<GraphicsDriver>(New OpenGLGraphicsDriver()));
}

void OpenGLModuleTerm()
{
    SetGraphicsDriver(nullptr);
}

DEFINE_MODULE() {
    "OpenGL",
    OpenGLModuleInit,
    OpenGLModuleTerm,
};
