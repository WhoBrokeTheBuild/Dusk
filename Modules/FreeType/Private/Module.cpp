#include <Dusk/Module.hpp>

#include <Dusk/Log.hpp>

namespace Dusk::FreeType {

void ModuleInit() {
}

void ModuleTerm() {
}

DEFINE_MODULE() {
    "FreeType",
    ModuleInit,
    ModuleTerm,
};

} // namespace Dusk::FreeType