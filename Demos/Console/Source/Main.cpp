#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Graphics/TransformData.hpp>
#include <Dusk/Scene/AxisComponent.hpp>
#include <Dusk/Scene/MeshComponent.hpp>
#include <Dusk/Scene/Entity.hpp>
#include <Dusk/Scene/Scene.hpp>
#include <Dusk/Scene/Camera.hpp>

#include <Dusk/Event.hpp>

#include <thread>

int main(int argc, char** argv)
{
    Dusk::SetApplicationName("Console");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    Dusk::Initialize(argc, argv);
    Dusk::InitScriptConsole();

    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        Dusk::GraphicsDriver * gfx = Dusk::GetGraphicsDriver();

        if (gfx) {
            gfx->ProcessEvents();
        }

        Dusk::UpdateScriptConsole();

        if (gfx) {
            gfx->SwapBuffers();
        }
    }

    Dusk::TermScriptConsole();

    Dusk::Terminate();

    return 0;
}
