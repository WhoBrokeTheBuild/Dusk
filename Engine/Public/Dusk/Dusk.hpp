#ifndef DUSK_HPP
#define DUSK_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Version.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Path.hpp>

#include <functional>

namespace Dusk {

DUSK_ENGINE_API
bool Initialize(int argc, char ** argv);

DUSK_ENGINE_API
void Terminate();

DUSK_ENGINE_API
void SetRunning(bool running);

DUSK_ENGINE_API
bool IsRunning();

DUSK_ENGINE_API
bool Run(std::function<void()> update);

DUSK_ENGINE_API
bool RunScriptFile(const Path& path, bool useAssetPath = true);

DUSK_ENGINE_API
bool RunScriptString(const string& code);

DUSK_ENGINE_API
Version GetVersion();

DUSK_ENGINE_API
void SetApplicationName(const string& name);

DUSK_ENGINE_API
string GetApplicationName();

DUSK_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch);

DUSK_ENGINE_API
void SetApplicationVersion(const Version& version);

DUSK_ENGINE_API
Version GetApplicationVersion();

} // namespace Dusk

#endif // DUSK_HPP
