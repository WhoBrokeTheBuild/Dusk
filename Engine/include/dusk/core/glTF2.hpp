#ifndef DUSK_GLTF2_HPP
#define DUSK_GLTF2_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/scene/Scene.hpp>

#include <string>
#include <vector>
#include <memory>

namespace dusk {

namespace glTF2 {

std::vector<std::unique_ptr<Actor>> LoadSceneFromFile(const std::string& filename);

std::vector<std::shared_ptr<Mesh>> LoadPrimitivesFromFile(const std::string& filename);

} // namespace glTF2

} // namespace dusk

#endif // DUSK_GLTF2_HPP