#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

#include <Dusk/Config.hpp>

#include <string>
#include <algorithm>

namespace Dusk {

using std::string;
using std::string_view;

// TODO: Maybe later
// typedef std::string String;
// typedef std::string_view StringView;

inline bool StringEqualCaseInsensitive(string_view a, string_view b) {
    return std::equal(
        a.begin(), a.end(),
        b.begin(), b.end(),
        [](char a, char b) {
            return tolower(a) == tolower(b);
        }
    );
}

} // namespace Dusk

#endif // DUSK_STRING_HPP