#ifndef DUSK_UTIL_HPP
#define DUSK_UTIL_HPP

#include <Dusk/Config.hpp>

#include <string>

namespace Dusk {

DUSK_ENGINE_API
std::string GetDirname(std::string);

DUSK_ENGINE_API
std::string GetBasename(std::string);

DUSK_ENGINE_API
std::string GetExtension(std::string);

} // namespace Dusk

#endif // DUSK_UTIL_HPP