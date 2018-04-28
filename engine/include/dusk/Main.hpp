#ifndef DUSK_MAIN_HPP
#define DUSK_MAIN_HPP

#include <dusk/Dusk.hpp>

typedef void (*DuskSetupFn)(dusk::App *);
extern "C" void _DuskSetup(dusk::App * app);

#endif // DUSK_MAIN_HPP
