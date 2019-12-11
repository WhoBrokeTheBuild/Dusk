#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/ShaderImporter.hpp>

int main(int argc, char** argv) 
{
    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion(1, 0, 0);

    Dusk::Initialize(argc, argv);
    
    if (!Dusk::LoadModuleArray({
        "DuskSTBI",
        "DuskOpenGL",
        // "DuskVulkan",
    })) {
        return 1;
    }

    auto gfx = Dusk::GetGraphicsDriver();

    auto tex = gfx->CreateTexture();
    tex->LoadFromFile("Assets/models/teapot.png");
    delete tex;

    gfx->SetWindowSize({ 1024, 768 });

    Dusk::SetRunning(true);
    while (Dusk::IsRunning()) {
        gfx->ProcessEvents();
        gfx->SwapBuffers();
    }

    Dusk::Terminate();

    return 0;
}
