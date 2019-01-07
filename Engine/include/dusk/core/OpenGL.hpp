#ifndef DUSK_OPENGL_HPP
#define DUSK_OPENGL_HPP

#ifndef DUSK_MIN_OPENGL_VERSION_MAJOR
#define DUSK_MIN_OPENGL_VERSION_MAJOR 4
#endif

#ifndef DUSK_MIN_OPENGL_VERSION_MINOR
#define DUSK_MIN_OPENGL_VERSION_MINOR 1
#endif

#define WIN32_LEAN_AND_MEAN
#include <glad/glad.h>
#undef CreateWindow

#include <SDL.h>

#endif // DUSK_OPENGL_HPP