#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/ScriptConsole.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/TransformData.hpp>
#include <Dusk/Scene/AxisComponent.hpp>
#include <Dusk/Scene/MeshComponent.hpp>
#include <Dusk/Scene/Entity.hpp>
#include <Dusk/Scene/Scene.hpp>
#include <Dusk/Scene/Camera.hpp>

#include <Dusk/Event.hpp>

#include <thread>

void testFunc(const Dusk::WindowResizedEventData * data)
{
    DuskLogInfo("testFunc %s", 
        glm::to_string(data->Size));
}

int main(int argc, char** argv)
{
    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    Dusk::Initialize(argc, argv);
    if (!Dusk::RunScriptFile("Assets/Scripts/Main.py")) {
        DuskLogError("Failed to load Main.py");
        return 1;
    }

    Dusk::ScriptConsole::Initialize();

    Dusk::UpdateContext updateContext;
    Dusk::RenderContext renderContext;

    {
        Dusk::Scene scene;
        Dusk::Camera camera;
        camera.SetPosition({ 10, 10, 10 });
        camera.SetLookAt({ 0, 0, 0 });

        // scene.AddComponent(std::make_unique<Dusk::AxisComponent>());

        auto transformData = renderContext.GetTransformData();
        transformData->View = camera.GetView();
        transformData->Projection = camera.GetProjection();

        // if (!scene.LoadFromFile("Models/cube.obj")) {
        //     DuskLogError("Failed to load Models/cube.obj");
        // }

        auto actor = scene.AddChild(std::make_unique<Dusk::Entity>());
        auto mesh = std::make_unique<Dusk::MeshComponent>();
        mesh->LoadFromFile("Assets/Models/crate/crate.obj");
        actor->AddComponent(std::move(mesh));

        auto gfx = Dusk::GetGraphicsDriver();

        Dusk::WindowResizedEventData testData;
        testData.Size = { 1024, 768 };
        gfx->WindowResizedEvent.Call(&testData);

        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({
           "flat.vert",
           "flat.frag",
        });

        Dusk::SetRunning(true);

        while (Dusk::IsRunning()) {
            gfx->ProcessEvents();

            scene.Update(&updateContext);

            Dusk::ScriptConsole::Update();

            shader->Bind();
            scene.Render(&renderContext);

            gfx->SwapBuffers();
        }
    }

    Dusk::ScriptConsole::Terminate();
    Dusk::Terminate();

    return 0;
}
