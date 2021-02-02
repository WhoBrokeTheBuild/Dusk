#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

#include <Dusk/Config.hpp>

#include <string>
#include <algorithm>

namespace Dusk {

using std::string;

inline bool StringEqualCaseInsensitive(const string& a, const string& b) {
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