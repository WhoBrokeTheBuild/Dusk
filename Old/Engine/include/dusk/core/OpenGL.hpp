#ifndef DUSK_OPENGL_HPP
#define DUSK_OPENGL_HPP

#ifndef DUSK_MIN_OPENGL_VERSION_MAJOR
#define DUSK_MIN_OPENGL_VERSION_MAJOR 4
#endif

#ifndef DUSK_MIN_OPENGL_VERSION_MINOR
#define DUSK_MIN_OPENGL_VERSION_MINOR 1
#endif

#include <glad/glad.h>

#undef CreateWindow
#undef min
#undef max
#undef near
#undef far

#include <SDL.h>

#endif // DUSK_OPENGL_HPP
