#ifndef DUSK_HPP
#define DUSK_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

#include <string>

namespace Dusk {

void Initialize(int argc, char ** argv);

void Terminate();

void RunScript(const std::string& filename);

}

#endif // DUSK_HPP
