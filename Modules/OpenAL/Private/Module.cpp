#include <Dusk/Module.hpp>

namespace Dusk::OpenAL {

void ModuleInit() {
}

void ModuleTerm() {

}

DEFINE_MODULE() {
    "OpenAL",
    ModuleInit,
    ModuleTerm,
};

}
