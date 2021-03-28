#include <Dusk/MediaType.hpp>

#include <unordered_map>

namespace Dusk {

static std::unordered_map<string, string> _MediaTypeByExtension;

void SetExtensionForMediaType(string ext, string mediaType)
{
    // TODO: lowercase ext and mimeType
    _MediaTypeByExtension[ext] = mediaType;
}

string GetMediaTypeFromExtension(string ext)
{
    auto it = _MediaTypeByExtension.find(ext);
    if (it != _MediaTypeByExtension.end()) {
        return it->second;
    }

    return "application/octet-stream";
}

} // namespace Dusk