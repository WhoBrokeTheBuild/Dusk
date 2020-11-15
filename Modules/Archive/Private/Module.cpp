#include <Dusk/Module.hpp>

namespace Dusk::Archive {

void ModuleInit() {
}

void ModuleTerm() {

}

DEFINE_MODULE() {
    "Archive",
    ModuleInit,
    ModuleTerm,
};

}