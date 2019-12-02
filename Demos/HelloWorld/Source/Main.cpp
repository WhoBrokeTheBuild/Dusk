#include <Dusk/Dusk.hpp>

#include <Dusk/Module.hpp>
#include <Dusk/Drivers/GraphicsDriver.hpp>
#include <Dusk/Loaders/TextureLoader.hpp>

int main(int argc, char** argv) 
{
    Dusk::Initialize(argc, argv);

    if (!Dusk::LoadModule("DuskSTBI")) {
        return 1;
    }
    
    if (!Dusk::LoadModule("DuskOpenGL")) {
        return 1;
    }

    auto gfx = Dusk::GetGraphicsDriver();
    auto txld = Dusk::GetTextureLoader();

    gfx->SetTitle("Hello, World!");
    gfx->SetSize({ 1024, 768 });

    Dusk::RunScript("Assets/scripts/Hello.py");

    auto data = txld->Load("Assets/models/teapot.png");
    printf("%zu x %zu\n", data.Width, data.Height);
    data.Free();

    Dusk::SetRunning(true);
    while (Dusk::IsRunning()) {
        gfx->ProcessEvents();
        gfx->SwapBuffers();
    }

    Dusk::Terminate();

    return 0;
}
