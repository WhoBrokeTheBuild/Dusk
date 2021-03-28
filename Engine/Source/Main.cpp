#include <Dusk/Dusk.hpp>
#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>
#include <Dusk/JSON.hpp>

#include <fstream>

using namespace Dusk;

// #if defined(DUSK_PLATFORM_UWP)

// [Platform::MTAThread]
// int WINAPIV main(Platform::Array<Platform::String^>^ params)
// {
//     Initialize(params);

// #elif defined(DUSK_PLATFORM_WINDOWS)

// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR lpCmdLine, int nCmdShow)
// {
//     Initialize(hInstance, lpCmdLine, nCmdShow);

// #else

int main(int argc, char ** argv)
{
    if (argc > 2) {
        LogInfo(DUSK_ANCHOR, "{} {}", argv[1], argv[2]);
    }

    std::ifstream file;
    file.open(argv[1], std::ios::in | std::ios::binary);

    json data;
    file >> data;

    LogInfo(DUSK_ANCHOR, "{} ({})", data["name"], data["version"]);
    
    auto modules = data["modules"].get<std::vector<std::string>>();
    auto assetPath = data["assetPath"].get<std::vector<std::string>>();
    auto modulePath = data["modulePath"].get<std::vector<std::string>>();

    auto config = data["configurations"];
    if (config.find(argv[2]) != config.end()) {
        auto configModules = config[argv[2]]["modules"].get<std::vector<std::string>>();
        modules.insert(modules.end(), configModules.begin(), configModules.end());
    }
    else {
        LogError(DUSK_ANCHOR, "Unable to find Configuration {}", argv[2]);
    }

    LogInfo(DUSK_ANCHOR, "{}", modules);

    Initialize(argc, argv);

    LoadModule("DuskOpenGL");

// #endif

    LogInfo(DUSK_ANCHOR, "Hello?");

    Run([]() { });

    Terminate();

    return 0;
}