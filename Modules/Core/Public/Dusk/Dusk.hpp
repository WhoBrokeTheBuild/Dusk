#ifndef DUSK_HPP
#define DUSK_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

#include <string>

namespace Dusk {

DUSK_CORE_API void Initialize(int argc, char ** argv);

DUSK_CORE_API  void Terminate();

DUSK_CORE_API  void SetRunning(bool running);

DUSK_CORE_API  bool IsRunning();

DUSK_CORE_API  void RunScript(const std::string& filename);

}

#endif // DUSK_HPP
