#ifndef DUSK_FONT_HPP
#define DUSK_FONT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

namespace Dusk {

class DUSK_CORE_API Font
{
public:

    DISALLOW_COPY_AND_ASSIGN(Font);

    bool LoadFromFile(const std::string& filename);

}; // class Font

} // namespace Dusk

#endif // DUSK_FONT_HPP