#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/Model.hpp>
#include <Dusk/Scene/Scene.hpp>

int main(int argc, char** argv)
{
    Dusk::Initialize(argc, argv);
    Dusk::RunScript("Scripts/Main.py");

    Dusk::Scene scene;
    Dusk::Entity * entity = scene.AddChild(std::make_unique<Dusk::Entity>());
    DuskLog("Entity at %s", glm::to_string(entity->GetPosition()));

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
