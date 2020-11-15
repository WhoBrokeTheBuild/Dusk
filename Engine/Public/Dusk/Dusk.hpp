#ifndef DUSK_HPP
#define DUSK_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Version.hpp>
#include <Dusk/Math.hpp>

#include <string>

namespace Dusk {

DUSK_CORE_API
void Initialize(int argc, char ** argv);

DUSK_CORE_API
void Terminate();

DUSK_CORE_API
void SetRunning(bool running);

DUSK_CORE_API
bool IsRunning();

DUSK_CORE_API
bool RunScriptFile(const std::string& filename);

DUSK_CORE_API
bool RunScriptString(const std::string& code);

DUSK_CORE_API
void InitScriptConsole();

DUSK_CORE_API
void TermScriptConsole();

DUSK_CORE_API
void UpdateScriptConsole();

DUSK_CORE_API
inline Version GetVersion() {
    return Version(DUSK_VERSION_MAJOR, DUSK_VERSION_MINOR, DUSK_VERSION_PATCH);
}

DUSK_CORE_API
void SetApplicationName(const std::string& name);

DUSK_CORE_API
std::string GetApplicationName();

DUSK_CORE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch);

DUSK_CORE_API
void SetApplicationVersion(const Version& version);

DUSK_CORE_API
Version GetApplicationVersion();

}

#endif // DUSK_HPP
