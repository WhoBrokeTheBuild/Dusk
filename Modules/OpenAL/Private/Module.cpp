#include <Dusk/Module.hpp>

namespace Dusk::OpenAL {

void ModuleInit() {
}

void ModuleTerm() {

}

DUSK_MODULE() {
    "OpenAL",
    ModuleInit,
    ModuleTerm,
};

}
