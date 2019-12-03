#ifndef DUSK_JSON_HPP
#define DUSK_JSON_HPP

#define JSON_NOEXCEPTION
#include <nlohmann/json.hpp>

namespace Dusk {

typedef nlohmann::basic_json<
    std::map,
    std::vector,
    std::string,
    bool,
    std::int64_t,
    std::uint64_t,
    float,
    std::allocator> json;

} // namespace Dusk

#endif // DUSK_JSON_HPP