#ifndef DUSK_BASE64_HPP
#define DUSK_BASE64_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/Span.hpp>
#include <Dusk/String.hpp>
#include <Dusk/Optional.hpp>

#include <cstdint>

namespace dusk {

namespace Base64 {

    void test();

    DUSK_API
    String Encode(Span<const uint8_t> data);

    inline String Encode(StringView string) {
        return Encode(Span<const uint8_t>(
            reinterpret_cast<const uint8_t *>(string.data()),
            string.size()
        ));
    }

    DUSK_API
    Optional<List<uint8_t>> Decode(StringView input);

} // namespace Base64

} // namespace dusk

#endif // DUSK_BASE64_HPP