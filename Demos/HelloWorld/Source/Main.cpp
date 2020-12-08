#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/ScriptConsole.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/TransformData.hpp>
#include <Dusk/AxisComponent.hpp>
#include <Dusk/MeshComponent.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/Scene.hpp>
#include <Dusk/Camera.hpp>

#include <Dusk/Event.hpp>

#include <thread>

void run()
{
    Dusk::UpdateContext updateContext;
    Dusk::RenderContext renderContext;

    Dusk::Scene scene;
    // scene.LoadFromFile("Assets/Models/TestScene.glb");
    // scene.LoadFromFile("Assets/Models/WaterBottle.glb");
    // scene.LoadFromFile("Assets/Models/DamagedHelmet.glb");
    // scene.LoadFromFile("Assets/Models/SciFiHelmet/SciFiHelmet.gltf");
    
    Dusk::Camera camera;
    camera.SetPosition({ 10, 10, 10 });
    camera.SetLookAt({ 0, 0, 0 });

    // scene.AddComponent(std::make_unique<Dusk::AxisComponent>());

    auto transformData = renderContext.GetTransformData();
    transformData->View = camera.GetView();
    transformData->Projection = camera.GetProjection();

    // if (!scene.LoadFromFile("Assets/Models/cube.obj")) {
    //     DuskLogError("Failed to load Assets/Models/cube.obj");
    // }

    // auto actor = scene.AddChild(std::make_unique<Dusk::Entity>());
    // auto mesh = std::make_unique<Dusk::MeshComponent>();
    // mesh->LoadFromFile("Assets/Models/crate/crate.obj");
    // actor->AddComponent(std::move(mesh));

    auto gfx = Dusk::GetGraphicsDriver();

    // Dusk::WindowResizedEventData testData;
    // testData.Size = { 1024, 768 };
    // gfx->WindowResizedEvent.Call(&testData);

    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "flat.vert",
        "flat.frag",
    })) {
        return;
    }

    // auto pipeline = gfx->CreatePipeline();
    // pipeline->SetShader(shader.get());
    // pipeline->Bind();

    Dusk::ScriptConsole::Initialize();

    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        gfx->ProcessEvents();

        scene.Update(&updateContext);

        Dusk::ScriptConsole::Update();

        // shader->Bind();
        scene.Render(&renderContext);

        gfx->SwapBuffers();
    }

    Dusk::ScriptConsole::Terminate();
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

    run();

    Dusk::Terminate();

    return 0;
}
