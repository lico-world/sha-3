#include "../include/hash_utils.h"

namespace hash {
namespace utils {
namespace bit_management {

uint64_t load64(const uint8_t* bytes)
{
    return (static_cast<uint64_t>(bytes[0])      ) |
            (static_cast<uint64_t>(bytes[1]) <<  8) |
            (static_cast<uint64_t>(bytes[2]) << 16) |
            (static_cast<uint64_t>(bytes[3]) << 24) |
            (static_cast<uint64_t>(bytes[4]) << 32) |
            (static_cast<uint64_t>(bytes[5]) << 40) |
            (static_cast<uint64_t>(bytes[6]) << 48) |
            (static_cast<uint64_t>(bytes[7]) << 56);
}

void store64(uint8_t* bytes, uint64_t lane)
{
    for(int i=0, k=0 ; i<8 ; i++, k+=8)
    {
        bytes[i] = static_cast<uint8_t>((lane >> k) & 0xFF);
    }
}

} // namespace bit_management

namespace permutations {

void keccakf(uint64_t state[25])
{}

void theta(uint64_t state[25])
{}

void rho(uint64_t state[25])
{}

void pi(uint64_t state[25])
{}

void chi(uint64_t state[25])
{}

void iota(uint64_t state[25], unsigned round)
{}


} // namespace permutations

namespace sponge {

void absorb(uint64_t state[25], const uint8_t* input, size_t inlen, size_t rate)
{}

void absorbLastBlock(uint64_t state[25], const uint8_t* last, size_t lastLen, size_t rate)
{}

void squeeze(uint64_t state[25], uint8_t* output, size_t outlen, size_t rate)
{}


} // namespace sponge

} // namespace utils    
} // namespace hash