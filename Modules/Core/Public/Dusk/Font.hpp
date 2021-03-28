#ifndef DUSK_FONT_HPP
#define DUSK_FONT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/String.hpp>

namespace Dusk {

class DUSK_CORE_API Font
{
public:

    DISALLOW_COPY_AND_ASSIGN(Font);

    bool LoadFromFile(string_view filename);

}; // class Font

} // namespace Dusk

#endif // DUSK_FONT_HPP