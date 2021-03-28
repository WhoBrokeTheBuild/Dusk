#ifndef DUSK_MEDIA_TYPE_HPP
#define DUSK_MEDIA_TYPE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/String.hpp>

namespace Dusk {

void SetExtensionForMediaType(string ext, string mediaType);

string GetMediaTypeFromExtension(string ext);

} // namespace Dusk

#endif // DUSK_MEDIA_TYPE_HPP