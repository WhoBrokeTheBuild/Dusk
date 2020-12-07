#include <Dusk/Module.hpp>

using namespace Dusk;

void OpenALModuleInit()
{
}

void OpenALModuleTerm()
{

}

DEFINE_MODULE() {
    "OpenAL",
    OpenALModuleInit,
    OpenALModuleTerm,
};
