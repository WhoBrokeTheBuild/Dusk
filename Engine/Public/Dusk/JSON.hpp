#ifndef DUSK_JSON_HPP
#define DUSK_JSON_HPP

#include <Dusk/Config.hpp>
#include <Dusk/String.hpp>

#define JSON_NOEXCEPTION
#include <nlohmann/json.hpp>

namespace Dusk {

typedef nlohmann::basic_json<
    std::map,
    std::vector,
    string,
    bool,
    int,
    unsigned,
    float,
    std::allocator> json;

} // namespace Dusk

#endif // DUSK_JSON_HPP