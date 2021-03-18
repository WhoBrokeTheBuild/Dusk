#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/ScriptConsole.hpp>
#include <Dusk/GraphicsDriver.hpp>

#include <Dusk/Event.hpp>

#include <thread>

int main(int argc, char** argv)
{
    Dusk::AddLogFile("lastrun.log");

    Dusk::SetApplicationName("Console");
    Dusk::SetApplicationVersion({ 1, 0, 0 });

    if (!Dusk::Initialize(argc, argv)) {
        return 1;
    }

    Dusk::ScriptConsole::Initialize();

    Dusk::SetRunning(true);

    while (Dusk::IsRunning()) {
        auto gfx = Dusk::GraphicsDriver::GetInstance();

        if (gfx) {
            gfx->ProcessEvents();
        }

        Dusk::ScriptConsole::Update();

        if (gfx) {
            gfx->Render();
        }
    }

    Dusk::ScriptConsole::Terminate();
    Dusk::Terminate();

    return 0;
}
