#include <Dusk/Graphics/ShaderImporter.hpp>

namespace Dusk {

static IShaderImporter * _ShaderImporter = nullptr;

DUSK_CORE_API
void SetShaderImporter(IShaderImporter * importer)
{
    if (_ShaderImporter) {
        delete _ShaderImporter;
    }
    _ShaderImporter = importer;
}

DUSK_CORE_API
IShaderImporter * GetShaderImporter()
{
    return _ShaderImporter;
}

} // namespace Dusk