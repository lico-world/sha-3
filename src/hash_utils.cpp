#include "../include/hash_utils.h"

namespace hash {
namespace utils {
namespace bit_management {

uint64_t load64(const uint8_t* bytes)
{
    return 0;
}

void store64(uint8_t* bytes, uint64_t lane)
{}

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