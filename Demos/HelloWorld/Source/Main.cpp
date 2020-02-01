#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/TransformData.hpp>
#include <Dusk/Scene/AxisComponent.hpp>
#include <Dusk/Scene/Scene.hpp>
#include <Dusk/Scene/Camera.hpp>

#include <Dusk/Event.hpp>

#include <thread>

void testFunc(const Dusk::WindowResizedEventData * data)
{
    DuskLogInfo("testFunc %s %s", 
        glm::to_string(data->Delta), 
        glm::to_string(data->Size));
}

int main(int argc, char** argv)
{
    Dusk::SetApplicationName("HelloWorld");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    Dusk::Initialize(argc, argv);
    Dusk::RunScriptFile("Scripts/Main.py");

    Dusk::LoadModule("DuskAssimp");
    
    Dusk::UpdateContext updateContext;
    Dusk::RenderContext renderContext;

    Dusk::Scene scene;
    Dusk::Camera camera;
    camera.SetPosition({ 10, 10, 10 });
    camera.SetLookAt({ 0, 0, 0 });

    // scene.AddComponent(std::make_unique<Dusk::AxisComponent>());

    auto transformData = renderContext.GetTransformData();
    transformData->View = camera.GetView();
    transformData->Projection = camera.GetProjection();

    if (!scene.LoadFromFile("Models/TestScene.glb")) {
        DuskLogError("Failed to load Models/TestScene.glb");
    }

    {
        auto gfx = Dusk::GetGraphicsDriver();

        Dusk::WindowResizedEventData testData;
        testData.Size = { 1024, 768 };
        testData.Delta = { 0, 0 };
        gfx->WindowResizedEvent.Call(&testData);

        auto shader = gfx->CreateShader();
        shader->LoadFromFiles({
           "flat.vert",
           "flat.frag",
        });

        Dusk::SetRunning(true);

        std::thread pyPrompt([]{
            while (Dusk::IsRunning()) {
                PyRun_InteractiveLoop(stdin, "");
            }
        });

        pyPrompt.detach();

        while (Dusk::IsRunning()) {
            gfx->ProcessEvents();

            scene.Update(&updateContext);

            shader->Bind();
            scene.Render(&renderContext);

            gfx->SwapBuffers();
        }
    }

    Dusk::Terminate();

    return 0;
}
