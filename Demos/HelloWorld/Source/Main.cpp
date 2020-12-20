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
    // Dusk::LoadModule("DuskOpenGL");
    Dusk::LoadModule("DuskVulkan");
    // Dusk::LoadModule("DuskDirectX");

    auto gfx = Dusk::GetGraphicsDriver();

    // auto shader = gfx->CreateShader();
    // if (!shader->LoadFromFiles({
    //     "flat.vert",
    //     "flat.frag",
    // })) {
    //     return;
    // }

    // auto meshImporters = Dusk::GetAllMeshImporters();
    // auto meshDatas = meshImporters[0]->LoadFromFile("sphere.obj");
    // auto mesh = Dusk::LoadMeshFromFile("monkey.obj");
    // auto meshDatas = meshImporters[0]->LoadFromFile("crate/crate.obj");
    // auto meshDatas = meshImporters[0]->LoadFromFile("BoomBox.glb");

    // auto pipeline = gfx->CreatePipeline(shader);

    Dusk::ScriptConsole::Initialize();

    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        gfx->ProcessEvents();

        Dusk::ScriptConsole::Update();

        // if (mesh) {
        //     mesh->Render();
        // }

        gfx->SwapBuffers();

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
    if (!Dusk::RunScriptFile("Main.py")) {
        DuskLogError("Failed to load Main.py");
        return 1;
    }

    run();

    Dusk::Terminate();

    return 0;
}
