#ifndef HASH_H
#define HASH_H

#include <cstdint>

namespace hash {

    void sha3(uint8_t* output, size_t outlen, const uint8_t* input, size_t inlen)
    {
        switch(outlen)
        {
            case 28:
            case 224:
                sha3_224(output, input, inlen);
                break;
            case 32:
            case 256:
                sha3_256(output, input, inlen);
                break;
            case 48:
            case 384:
                sha3_384(output, input, inlen);
                break;
            case 64:
            case 512:
                sha3_512(output, input, inlen);
                break;
            default:
                // Unsupported output length
                break;
        }
    }

    void sha3_224(uint8_t* output, const uint8_t* input, size_t inlen);
    void sha3_256(uint8_t* output, const uint8_t* input, size_t inlen);
    void sha3_384(uint8_t* output, const uint8_t* input, size_t inlen);
    void sha3_512(uint8_t* output, const uint8_t* input, size_t inlen);

} // namespace hash

#endif  // HASH_H
