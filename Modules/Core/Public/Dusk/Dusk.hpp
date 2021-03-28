#ifndef DUSK_HPP
#define DUSK_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Version.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

#include <functional>

namespace Dusk {

DUSK_CORE_API
bool Initialize(int argc, char ** argv);

DUSK_CORE_API
void Terminate();

DUSK_CORE_API
void LoadConfigurationFile(const Path& path, string_view configName);

DUSK_CORE_API
void SetRunning(bool running);

DUSK_CORE_API
bool IsRunning();

DUSK_CORE_API
bool Run(std::function<void()> update);

DUSK_CORE_API
bool RunScriptFile(const Path& path, bool useAssetPath = true);

DUSK_CORE_API
bool RunScriptString(const string& code);

DUSK_CORE_API
Version GetVersion();

DUSK_CORE_API
void SetApplicationName(const string& name);

DUSK_CORE_API
string GetApplicationName();

DUSK_CORE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch);

DUSK_CORE_API
void SetApplicationVersion(const Version& version);

DUSK_CORE_API
Version GetApplicationVersion();

} // namespace Dusk

#endif // DUSK_HPP
