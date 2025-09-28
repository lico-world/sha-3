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
namespace permutations_helpers {

const uint64_t rho_offsets[25] = {
    0, 1, 3, 6, 10,
    15, 21, 28, 36, 45,
    55, 2, 14, 27, 41,
    56, 8, 25, 43, 62,
    18, 39, 61, 20, 44};

const uint64_t RC[24] = {
    0x0000000000000001, 0x0000000000008082,
    0x800000000000808A, 0x8000000080008000,
    0x000000000000808B, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009,
    0x000000000000008A, 0x0000000000000088,
    0x0000000080008009, 0x000000008000000A,
    0x000000008000808B, 0x800000000000008B,
    0x8000000000008089, 0x8000000000008003,
    0x8000000000008002, 0x8000000000000080,
    0x000000000000800A, 0x800000008000000A,
    0x8000000080008081, 0x8000000000008080,
    0x0000000080000001, 0x8000000080008008};

} // namespace permutations_helpers

void keccakf(uint64_t state[25])
{
    // 64bits -> 12 + 2*log2(64) = 24 rounds
    for(int i=0 ; i<24 ; i++)
    {
        theta(state);
        rho(state);
        pi(state);
        chi(state);
        iota(state, i);
    }
}

void theta(uint64_t state[25])
{
    uint64_t* C = permutations_helpers::parities(state);

    uint64_t D[5];
    for(int i=0 ; i<5 ; i++)
    {
        D[i] = C[(i + 4) % 5] ^ bit_management::rotl64(C[(i + 1) % 5], 1);
    }

    for(int i=0 ; i<5 ; i++)
    {
        for(int j=0 ; j<5 ; j++)
        {
            state[j + 5*i] ^= D[j];
        }
    }
}

void rho(uint64_t state[25])
{
    for(int i=0 ; i<25 ; i++)
    {
        state[i] = bit_management::rotl64(state[i], permutations_helpers::rho_offsets[i]);
    }
}

void pi(uint64_t state[25])
{
    uint64_t initial_state[25];
    std::copy(state, state + 25, initial_state);

    for(int i=0 ; i<5 ; i++)
    {
        for(int j=0 ; j<5 ; j++)
        {
            state[5*((3*i+2*j)%5) + i] = initial_state[i*5 + j];
        }
    }
}

void chi(uint64_t state[25])
{
    uint64_t initial_state[25];
    std::copy(state, state + 25, initial_state);

    for(int i=0 ; i<25 ; i++)
    {
        state[i] = initial_state[i] ^ (~initial_state[((i+1)%5 + (i+1)/5*5)%25] & initial_state[((i+2)%5 + (i+2)/5*5)%25]);
    }
}

void iota(uint64_t state[25], unsigned round)
{
    state[0] ^= permutations_helpers::RC[round];
}


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