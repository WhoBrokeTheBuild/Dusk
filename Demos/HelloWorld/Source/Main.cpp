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
    Dusk::LoadModule("DuskGLTF2");
    // Dusk::LoadModule("DuskFreeType");

    const char * envGraphicsDriver = getenv("DUSK_GRAPHICS_DRIVER");
    
    if (!envGraphicsDriver) {
        envGraphicsDriver = "Vulkan";
    }

    if (strcmp(envGraphicsDriver, "OpenGL") == 0) {
        Dusk::LoadModule("DuskOpenGL");
    }
    else if (strcmp(envGraphicsDriver, "DirectX") == 0) {
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
    camera.SetPosition(glm::vec3(2.0f));
    camera.SetLookAt({ 0, 0, 0 });

    static float rotation = 0.0f;
    rotation += 0.01f;
    Dusk::Scene scene;
    Dusk::SetCurrentScene(&scene);


    // auto shader = gfx->CreateShader();
    // shader->LoadFromFiles({
    //     "Dusk/Debug/Material.vert",
    //     "Dusk/Debug/MaterialEmissiveMap.frag",
    // });

    // gfx->GetDefaultPipeline()->SetShader(shader);

    // auto mesh = Dusk::LoadMeshFromFile("WaterBottle/WaterBottle.gltf");
    // camera.SetPosition(glm::vec3(0.2f));

    // auto mesh = Dusk::LoadMeshFromFile("SciFiHelmet/SciFiHelmet.gltf");
    // camera.SetPosition(glm::vec3(2.0f));

    auto mesh = Dusk::LoadMeshFromFile("DamagedHelmet/DamagedHelmet.gltf");
    camera.SetPosition(glm::vec3(1.5f));

    // auto mesh = Dusk::LoadMeshFromFile("BoomBox/BoomBox.gltf");
    // camera.SetPosition(glm::vec3(1.0f));

    if (!mesh) {
        return;
    }

    auto entity = std::unique_ptr<Dusk::Entity>(New Dusk::Entity());
    Dusk::Entity * tmpEntity = entity.get();

    auto meshComponent = std::unique_ptr<Dusk::MeshComponent>(New Dusk::MeshComponent());
    meshComponent->SetMesh(mesh);
    entity->AddComponent(std::move(meshComponent));

    scene.AddChild(std::move(entity));

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
