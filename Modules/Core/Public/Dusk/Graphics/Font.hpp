#ifndef DUSK_FONT_HPP
#define DUSK_FONT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

namespace Dusk {

class DUSK_CORE_API IFont
{
public:

    DISALLOW_COPY_AND_ASSIGN(IFont);

    bool LoadFromFile(const std::string& filename);

}; // class IFont

} // namespace Dusk

#endif // DUSK_FONT_HPP