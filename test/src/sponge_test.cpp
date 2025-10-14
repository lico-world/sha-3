#include "../../include/hash_utils.h"

#include <gtest/gtest.h>

// Mocks / stubs for determinism
namespace bit_management
{
    static inline uint64_t load64(const uint8_t* x)
    {
        uint64_t r = 0;
        for (int i = 0; i < 8; ++i) r |= (uint64_t)x[i] << (8 * i);
        return r;
    }
}

namespace permutations
{
    static void keccakf(uint64_t state[25])
    {
        state[0] ^= 0xDEADBEEFDEADBEEF;
    }
}

class AbsorbLastBlockTest : public ::testing::Test
{
protected:
    uint64_t state[25];
    void SetUp() override
    {
        for(int i = 0; i < 25; i++) state[i] = 0;
    }
};

TEST_F(AbsorbLastBlockTest, EmptyLastBlock) {
    uint8_t input[1] = {}; // unused
    hash::utils::sponge::absorbLastBlock(state, input, 0, 64); // 64 bits (8 bytes) rate

    // Expect padding pattern: 0x01 at byte 0, 0x80 at byte 7
    uint64_t expected = 0x8000000000000001ULL; // little-endian
    EXPECT_EQ(state[0] & 0xFFFFFFFFFFFFFFFFULL, expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbLastBlockTest, SmallLastBlock) {
    uint8_t input[3] = {0xAA, 0xBB, 0xCC};
    hash::utils::sponge::absorbLastBlock(state, input, 3, 64);

    uint8_t temp[8] = {0xAA, 0xBB, 0xCC, 0x01, 0x00, 0x00, 0x00, 0x80};
    uint64_t expected = bit_management::load64(temp);
    EXPECT_EQ(state[0], expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbLastBlockTest, NearFullBlock) {
    uint8_t input[7] = {1, 2, 3, 4, 5, 6, 7};
    hash::utils::sponge::absorbLastBlock(state, input, 7, 64);

    uint8_t temp[8] = {1, 2, 3, 4, 5, 6, 7, 0x81};
    uint64_t expected = bit_management::load64(temp);
    EXPECT_EQ(state[0], expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbLastBlockTest, TwoLanes) {
    uint8_t input[10];
    for (int i = 0; i < 10; i++) input[i] = i + 1;

    hash::utils::sponge::absorbLastBlock(state, input, 10, 128); // 16 bytes

    uint8_t temp[16] = {};
    std::copy(input, input + 10, temp);
    temp[10] = 0x01;
    temp[15] |= 0x80;

    uint64_t expected0 = bit_management::load64(temp);
    uint64_t expected1 = bit_management::load64(temp + 8);

    EXPECT_EQ(state[0], expected0 ^ 0xDEADBEEFDEADBEEF);
    EXPECT_EQ(state[1], expected1);
}

TEST_F(AbsorbLastBlockTest, PermutationApplied) {
    uint8_t input[2] = {0x11, 0x22};
    hash::utils::sponge::absorbLastBlock(state, input, 2, 64);
    EXPECT_NE(state[0], 0ULL);
}