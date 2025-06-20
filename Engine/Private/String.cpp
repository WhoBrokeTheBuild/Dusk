#include <Dusk/String.hpp>
#include <Dusk/Array.hpp>
#include <Dusk/Enumerate.hpp>

#include <format>

namespace dusk {

DUSK_API
List<String> StringSplit(StringView str, StringView delim)
{
    List<String> strList;

    auto next = str.find(delim);
    while (next != StringView::npos) {
        strList.emplace_back(str.substr(0, next));
        str = str.substr(next + delim.size());
        next = str.find(delim);
    }

    strList.emplace_back(str);

    return strList;
}

DUSK_API
String StringJoin(List<StringView> strList, StringView delim)
{
    size_t totalSize = delim.size() * (strList.size() - 1);
    for (const auto& str : strList) {
        totalSize += str.size();
    }

    String result;
    result.reserve(totalSize);

    for (const auto& [index, str] : Enumerate(strList)) {
        result += str;
        if (index < (strList.size() - 1)) {
            result += delim;
        }
    }

    return result;
}

DUSK_API
String FormatBytesHumanReadable(uint64_t bytes)
{
    static const Array<const char *, 7> SUFFIX_LIST = {
        "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"
    };

    double temp = bytes;
    std::size_t suffixIndex = 0;
    while (temp >= 1024.0) {
        ++suffixIndex;
        temp /= 1024;
    }

    if (suffixIndex >= SUFFIX_LIST.size()) {
        return std::format("{}B", bytes);
    }

    return std::format("{:.3f}{}", temp, SUFFIX_LIST[suffixIndex]);
}

} // namespace dusk