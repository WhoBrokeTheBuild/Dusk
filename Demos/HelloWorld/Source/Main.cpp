#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/ScriptConsole.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/ShaderTransform.hpp>
#include <Dusk/AxisComponent.hpp>
#include <Dusk/MeshComponent.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/Scene.hpp>
#include <Dusk/Camera.hpp>
#include <Dusk/MeshImporter.hpp>
#include <Dusk/Path.hpp>

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
        graphicsDriver = "Vulkan";
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
    Dusk::ShaderTransform * transformData = renderCtx->GetShaderTransform();

    Dusk::Camera camera;
    camera.SetFOVX(45.0f);
    camera.SetPosition({ 3, 3, 3 });
    camera.SetLookAt({ 0, 0, 0 });

    static float rotation = 0.0f;
    rotation += 0.01f;

    auto shader = gfx->CreateShader();
    if (!shader->LoadFromFiles({
        "Dusk/DebugNormalColor.vert",
        "Dusk/DebugNormalColor.frag",
    })) {
        return;
    }

    auto pipeline = gfx->CreatePipeline(shader);

    Dusk::Scene scene;
    Dusk::SetCurrentScene(&scene);

    auto mesh = Dusk::LoadMeshFromFile("monkey.obj");
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

    // Force ResetSwapChain
    gfx->SetWindowSize({ 1024, 768 });

    Dusk::ScriptConsole::Initialize();

    Dusk::Run([&]() {
        Dusk::ScriptConsole::Update();

        transformData->View = camera.GetView();
        transformData->Projection = camera.GetProjection();

        glm::quat orient = tmpEntity->GetOrientation() * glm::angleAxis(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        tmpEntity->SetOrientation(orient);
    });

    Dusk::ScriptConsole::Terminate();
}

int main(int argc, char** argv)
{
    Dusk::AddLogFile("lastrun.log");

    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    if (!Dusk::Initialize(argc, argv)) {
        return 1;
    }

    // if (!Dusk::RunScriptFile("Main.py")) {
    //     DuskLogError("Failed to load Main.py");
    //     return 1;
    // }

    run();

    Dusk::Terminate();

    return 0;
}
