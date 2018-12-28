#ifndef DUSK_ASSET_TYPES_HPP
#define DUSK_ASSET_TYPES_HPP

#include "Config.hpp"

enum class AssetType : int {
    Model = 0,
    Material,
    Texture,
    Font,
    Script,
    Shader,
    Audio,
    Video,
    Unknown,
};

static const std::string AssetTypeNames[] = {
    "Model",
    "Material",
    "Texture",
    "Font",
    "Script",
    "Shader",
    "Audio",
    "Video",
    "Unknown",
};

#endif // DUSK_ASSET_TYPES_HPP
