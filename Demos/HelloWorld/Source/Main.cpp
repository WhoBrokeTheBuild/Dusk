#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Drivers/GraphicsDriver.hpp>
#include <Dusk/Importers/TextureImporter.hpp>

int main(int argc, char** argv) 
{
    Dusk::Initialize(argc, argv);
    
    if (!Dusk::LoadModuleArray({
        // "DuskAssimp",
        "DuskGLTF2",
        "DuskSTBI",
        "DuskOpenGL",
        // "DuskVulkan",
    })) {
        return 1;
    }

    auto gfx = Dusk::GetGraphicsDriver();
    auto png = Dusk::GetTextureImporter("png");

    gfx->SetWindowTitle("Hello, World!");
    gfx->SetWindowSize({ 1024, 768 });

    Dusk::RunScript("Assets/scripts/Hello.py");

    auto data = png->LoadFromFile("Assets/models/teapot.png");
    DuskLogInfo("Dimensions of teapot.png: (%zu, %zu)", data.Width, data.Height);
    data.Free();

    Dusk::SetRunning(true);
    while (Dusk::IsRunning()) {
        gfx->ProcessEvents();
        gfx->SwapBuffers();
    }

    Dusk::Terminate();

    return 0;
}
