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
#include <Dusk/MeshImporter.hpp>

#include <Dusk/Event.hpp>

#include <thread>

#include <chrono>

using namespace std::chrono;

void run()
{
    Dusk::LoadModule("DuskSTBI");
    Dusk::LoadModule("DuskTinyOBJ");
    // Dusk::LoadModule("DuskGLTF2");
    // Dusk::LoadModule("DuskFreeType");

    const char * graphicsDriver = getenv("DUSK_GRAPHICS_DRIVER");
    
    if (!graphicsDriver) {
        graphicsDriver = "DuskVulkan";
    }

    if (strcmp(graphicsDriver, "OpenGL") == 0) {
        Dusk::LoadModule("DuskOpenGL");
    }
    else if (strcmp(graphicsDriver, "DirectX") == 0) {
        Dusk::LoadModule("DuskDirectX");
    }
    else {
        Dusk::LoadModule("DuskVulkan");
    }

    auto gfx = Dusk::GetGraphicsDriver();
    if (!gfx) {
        return;
    }

    Dusk::RenderContext * renderCtx = gfx->GetRenderContext();
    Dusk::TransformData * transformData = renderCtx->GetTransformData();

    Dusk::Camera camera;
    camera.SetAspect(glm::vec2(640.0f, 480.0f));
    camera.SetFOVX(45.0f);
    camera.SetMode(Dusk::CameraMode::Perspective);
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    static float rotation = 0.0f;
    rotation += 0.01f;

    transformData->View = camera.GetView();
    transformData->Projection = camera.GetProjection();

    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "flat.vert",
        "flat.frag",
    })) {
        return;
    }

    auto pipeline = gfx->CreatePipeline(shader);

    Dusk::Scene scene;
    gfx->SetCurrentScene(&scene);

    auto mesh = Dusk::LoadMeshFromFile("cube.obj");
    mesh->SetPipeline(pipeline);

    auto entity = std::unique_ptr<Dusk::Entity>(New Dusk::Entity());
    Dusk::Entity * tmpEntity = entity.get();

    auto meshComponent = std::unique_ptr<Dusk::MeshComponent>(New Dusk::MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    scene.AddChild(std::move(entity));

    // auto meshImporters = Dusk::GetAllMeshImporters();
    // auto meshDatas = meshImporters[0]->LoadFromFile("sphere.obj");
    // auto mesh = Dusk::LoadMeshFromFile("monkey.obj");
    // auto meshDatas = meshImporters[0]->LoadFromFile("crate/crate.obj");
    // auto meshDatas = meshImporters[0]->LoadFromFile("BoomBox.glb");

    // auto pipeline = gfx->CreatePipeline(shader);

    Dusk::ScriptConsole::Initialize();

    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        gfx->Render();

        gfx->ProcessEvents();

        Dusk::ScriptConsole::Update();

        glm::quat orient = tmpEntity->GetOrientation() * glm::angleAxis(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        tmpEntity->SetOrientation(orient);

        std::this_thread::sleep_for(16ms);
    }

    Dusk::ScriptConsole::Terminate();
}

int main(int argc, char** argv)
{
    Dusk::AddLogFile("lastrun.log");

    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    Dusk::Initialize(argc, argv);
    // if (!Dusk::RunScriptFile("Main.py")) {
    //     DuskLogError("Failed to load Main.py");
    //     return 1;
    // }

    run();

    Dusk::Terminate();

    return 0;
}
