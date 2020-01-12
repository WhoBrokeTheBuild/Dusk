#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Scene/Scene.hpp>

int main(int argc, char** argv)
{
    Dusk::Initialize(argc, argv);
    Dusk::RunScript("Scripts/Main.py");

    Dusk::LoadModule("DuskAssimp");

    Dusk::Scene scene;

    if (!scene.LoadFromFile("Models/TestScene.glb")) {
        DuskLogError("Failed to load Models/TestScene.glb");
    }

    {
        auto gfx = Dusk::GetGraphicsDriver();

        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({
           "flat.vert",
           "flat.frag",
        });

        Dusk::SetRunning(true);
        while (Dusk::IsRunning()) {
            gfx->ProcessEvents();

            scene.Update(nullptr);

            shader->Bind();
            scene.Render(nullptr);

            gfx->SwapBuffers();
        }
    }

    Dusk::Terminate();

    return 0;
}
