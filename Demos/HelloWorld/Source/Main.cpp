#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/Model.hpp>
#include <Dusk/Scene/Camera.hpp>

int main(int argc, char** argv) 
{
    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion(1, 0, 0);

    Dusk::Initialize(argc, argv);
    Dusk::RunScript("Scripts/Main.py");

    {
        auto gfx = Dusk::GetGraphicsDriver();

        auto model = Dusk::Model();
        model.LoadFromFile("Models/crate/crate.obj");

        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({
            "Shaders/flat.vert",
            "Shaders/flat.frag",
        });

        auto tex = gfx->CreateTexture();
        tex->LoadFromFile("Models/crate/crate.png");

        gfx->SetWindowSize({ 1024, 768 });

        Dusk::SetRunning(true);
        while (Dusk::IsRunning()) {
            gfx->ProcessEvents();

            shader->Bind();
            model.Render();

            gfx->SwapBuffers();
        }
    }

    Dusk::Terminate();

    return 0;
}
