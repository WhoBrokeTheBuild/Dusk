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
    int,
    unsigned,
    float,
    std::allocator> json;

} // namespace Dusk

#endif // DUSK_JSON_HPP