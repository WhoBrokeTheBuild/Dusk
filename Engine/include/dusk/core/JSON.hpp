#ifndef DUSK_JSON_HPP
#define DUSK_JSON_HPP

#include <nlohmann/json.hpp>

namespace dusk {

typedef nlohmann::basic_json<
    std::map,
    std::vector,
    std::string,
    bool,
    std::int64_t,
    std::uint64_t,
    float,
    std::allocator> json;

} // namespace dusk

#endif // DUSK_JSON_HPP