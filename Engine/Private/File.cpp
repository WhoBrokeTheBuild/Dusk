#include <Dusk/File.hpp>

namespace dusk {

DUSK_API
bool File::Open(const Path& path, Mode mode /*= Mode::Default*/)
{
    _file.open(path, std::ios::openmode(mode));
    if (not _file) {
        return false;
    }

    // Disables skipping of leading whitespace
    _file.unsetf(std::ios::skipws);

    std::streamsize start = _file.tellg();
    _file.seekg(0, std::ios::end);
    _size = _file.tellg();
    _file.seekg(start, std::ios::beg);

    return bool(_file);
}

DUSK_API
void File::Close()
{
    _size = 0;
    _file.close();
}

} // namespace dusk
