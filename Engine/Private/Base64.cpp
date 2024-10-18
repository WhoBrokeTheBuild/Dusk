#include <Dusk/Base64.hpp>

#include <print>

namespace dusk {

// Inspired by https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594

DUSK_API
String Base64::Encode(Span<const uint8_t> data)
{
    static constexpr char TABLE[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/',
    };

    size_t dataLength = data.size();
    size_t outputLength = 4 * ((dataLength + 2) / 3);
    String output(outputLength, '\0');

    size_t dataIndex;
    char * pch = output.data();
    for (dataIndex = 0; (dataLength > 2) and (dataIndex < dataLength - 2); dataIndex += 3) {

        const uint8_t& A = data[dataIndex];
        const uint8_t& B = data[dataIndex + 1];
        const uint8_t& C = data[dataIndex + 2];

        // 24 bits AAAAAA|AABBBB|BBBBCC|CCCCCC
        uint32_t raw = (A << 16) | (B << 8) | C;

        *pch++ = TABLE[(raw >> 18)];
        *pch++ = TABLE[(raw >> 12) & 0b00111111];
        *pch++ = TABLE[(raw >>  6) & 0b00111111];
        *pch++ = TABLE[(raw >>  0) & 0b00111111];
    }

    if (dataIndex < dataLength) {

        const uint8_t& A = data[dataIndex];

        // (A & 11111100) >> 2
        *pch++ = TABLE[A >> 2];

        if (dataIndex == (dataLength - 1)) {

            // (A & 00000011) << 4
            *pch++ = TABLE[(A & 0b00000011) << 4];

            // Padding
            *pch++ = '=';
        }
        else {
            const uint8_t& B = data[dataIndex + 1];

            // AABBBB|BBBB
            uint16_t raw = ((A & 0b00000011) << 8) | B;

            *pch++ = TABLE[(raw >> 4)];
            *pch++ = TABLE[(raw << 2) & 0b00111111];
        }

        // Padding
        *pch++ = '=';
    }

    return output;
}

DUSK_API
Optional<List<uint8_t>> Base64::Decode(StringView input)
{
    static constexpr uint8_t TABLE[] = {
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
        64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
    };

    size_t inputLength = input.size();
    if (inputLength % 4 != 0) {
        return nullopt;
    }

    size_t outputLength = (inputLength / 4) * 3;
    if (input[inputLength - 1] == '=') {
        --outputLength;

        if (input[inputLength - 2] == '=') {
            --outputLength;
        }
    }

    List<uint8_t> output(outputLength, 0);

    size_t outputIndex = 0;
    for (size_t i = 0; i < inputLength; i += 4 ) {

        uint32_t raw = 0;
        for (size_t j = 0; j < 4; ++j) {
            raw <<= 6;
            if (input[i + j] != '=') {
                raw |= TABLE[(unsigned)input[i + j]];
            }
        }

        output[outputIndex++] = (raw >> 16);

        if (outputIndex < outputLength) {
            output[outputIndex++] = (raw >>  8) & 0b11111111;
        }

        if (outputIndex < outputLength) {
            output[outputIndex++] = (raw >>  0) & 0b11111111;
        }
    }

    return output;
}

} // namespace dusk