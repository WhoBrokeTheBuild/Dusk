#ifndef DUSK_GLTF2_HPP
#define DUSK_GLTF2_HPP

#include <dusk/Config.hpp>

#include <string>

namespace dusk {

namespace glTF2 {

bool LoadSceneFromFile(const std::string& filename);

bool LoadModelFromFile(const std::string& filename);

} // namespace glTF2

} // namespace dusk

#endif // DUSK_GLTF2_HPP