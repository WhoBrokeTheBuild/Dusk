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

int main(int argc, char** argv)
{
    Dusk::AddLogFile("lastrun.log");

    Dusk::SetApplicationName("Console");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    Dusk::Initialize(argc, argv);
    Dusk::ScriptConsole::Initialize();

    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        Dusk::GraphicsDriver * gfx = Dusk::GetGraphicsDriver();

        if (gfx) {
            gfx->ProcessEvents();
        }

        Dusk::ScriptConsole::Update();

        if (gfx) {
            gfx->SwapBuffers();
        }
    }

    Dusk::ScriptConsole::Terminate();
    Dusk::Terminate();

    return 0;
}
