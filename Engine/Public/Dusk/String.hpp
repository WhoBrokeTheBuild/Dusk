#ifndef DUSK_STRING_HPP
#define DUSK_STRING_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>

#include <cstdint>
#include <string>
#include <string_view>

namespace dusk {

using namespace std::string_view_literals;

using String = std::string;

using StringView = std::string_view;

inline bool StringContains(StringView haystack, StringView needle) {
    return (haystack.find(needle) != StringView::npos);
}

DUSK_API
List<String> StringSplit(StringView str, StringView delim);

DUSK_API
String StringJoin(List<StringView> strList, StringView delim);

DUSK_API
String FormatBytesHumanReadable(uint64_t bytes);

} // namespace dusk

#endif // DUSK_STRING_HPP