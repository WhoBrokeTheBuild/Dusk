#ifndef DUSK_CONFIG_HPP
#define DUSK_CONFIG_HPP

#define DUSK_VERSION_MAJOR 0
#define DUSK_VERSION_MINOR 0
#define DUSK_VERSION_PATCH 2
#define DUSK_REVISION "12e4e69bb509"
#define DUSK_VERSION_NUM ((DUSK_VERSION_MAJOR << 16) | \
                          (DUSK_VERSION_MINOR << 8) | \
                          DUSK_VERSION_PATCH)
#define DUSK_VERSION "0.0.2-12e4e69bb509"

// glad
#define WIN32_LEAN_AND_MEAN
#include <glad/glad.h>

// SDL2
#include <SDL.h>

// OpenAL
#include <AL/al.h>
#include <AL/alc.h>

// Ogg/Vorbis
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/norm.hpp>

// GSL
#include <gsl/gsl>

// SOL
#include <sol/sol.hpp>

// nlohmann json
#include <json.hpp>

// TinyObjLoader
#include <tinyobjloader/tiny_obj_loader.h>

// zlib
#include <zlib.h>

// STB
#include <stb/stb_image.h>
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>

#undef CreateWindow

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;   \
    TypeName& operator=(const TypeName&) = delete

#define DUSK_SYSTEM_NAME	"Linux"
#define DUSK_SYSTEM_VERSION "4.4.0-96-generic"

#endif // DUSK_CONFIG_HPP
