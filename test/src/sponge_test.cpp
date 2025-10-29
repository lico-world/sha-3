#include "../../include/hash_utils.h"

#include <gtest/gtest.h>

namespace hash {
namespace utils {
namespace permutations {

void keccakf(uint64_t state[25])
{
    state[0] ^= 0xDEADBEEFDEADBEEF;
}

}}}

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
    uint8_t input[1] = {};
    hash::utils::sponge::absorbLastBlock(state, input, 0, 64);

    uint64_t expected = 0x8000000000000006ULL;
    EXPECT_EQ(state[0] & 0xFFFFFFFFFFFFFFFFULL, expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbLastBlockTest, SmallLastBlock) {
    uint8_t input[3] = {0xAA, 0xBB, 0xCC};
    hash::utils::sponge::absorbLastBlock(state, input, 3, 64);

    uint8_t temp[8] = {0xAA, 0xBB, 0xCC, 0x06, 0x00, 0x00, 0x00, 0x80};
    uint64_t expected = hash::utils::bit_management::load64(temp);
    EXPECT_EQ(state[0], expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbLastBlockTest, NearFullBlock) {
    uint8_t input[7] = {1, 2, 3, 4, 5, 6, 7};
    hash::utils::sponge::absorbLastBlock(state, input, 7, 64);

    uint8_t temp[8] = {1, 2, 3, 4, 5, 6, 7, 0x86};
    uint64_t expected = hash::utils::bit_management::load64(temp);
    EXPECT_EQ(state[0], expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbLastBlockTest, TwoLanes) {
    uint8_t input[10];
    for (int i = 0; i < 10; i++) input[i] = i + 1;

    hash::utils::sponge::absorbLastBlock(state, input, 10, 128);

    uint8_t temp[16] = {};
    std::copy(input, input + 10, temp);
    temp[10] = 0x06;
    temp[15] |= 0x80;

    uint64_t expected0 = hash::utils::bit_management::load64(temp);
    uint64_t expected1 = hash::utils::bit_management::load64(temp + 8);

    EXPECT_EQ(state[0], expected0 ^ 0xDEADBEEFDEADBEEF);
    EXPECT_EQ(state[1], expected1);
}

TEST_F(AbsorbLastBlockTest, PermutationApplied) {
    uint8_t input[2] = {0x11, 0x22};
    hash::utils::sponge::absorbLastBlock(state, input, 2, 64);
    EXPECT_NE(state[0], 0ULL);
}

class AbsorbTest : public ::testing::Test
{
protected:
    uint64_t state[25];

    void SetUp() override
    {
        for (int i = 0; i < 25; i++) state[i] = 0;
    }
};

TEST_F(AbsorbTest, SingleFullBlock)
{
    uint8_t input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    hash::utils::sponge::absorb(state, input, sizeof(input), 64);

    uint64_t expected = hash::utils::bit_management::load64(input);
    EXPECT_EQ(state[0], expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbTest, MultipleFullBlocks)
{
    uint8_t input[16];
    for (int i = 0; i < 16; i++) input[i] = i + 1;

    hash::utils::sponge::absorb(state, input, sizeof(input), 64);

    uint64_t firstBlock = hash::utils::bit_management::load64(input);
    uint64_t secondBlock = hash::utils::bit_management::load64(input + 8);

    uint64_t expected = firstBlock ^ secondBlock;
    EXPECT_EQ(state[0], expected);
}

TEST_F(AbsorbTest, PartialFinalBlock)
{
    uint8_t input[5] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
    hash::utils::sponge::absorb(state, input, sizeof(input), 64);

    uint8_t temp[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x06, 0x00, 0x80};
    uint64_t expected = hash::utils::bit_management::load64(temp);
    EXPECT_EQ(state[0], expected ^ 0xDEADBEEFDEADBEEF);
}

TEST_F(AbsorbTest, EmptyInput)
{
    uint8_t input[1] = {};
    hash::utils::sponge::absorb(state, input, 0, 64);
    for (int i = 0; i < 25; i++)
        EXPECT_EQ(state[i], 0ULL);
}

TEST_F(AbsorbTest, TwoLaneRate)
{
    uint8_t input[16];
    for (int i = 0; i < 16; i++) input[i] = i + 1;

    hash::utils::sponge::absorb(state, input, sizeof(input), 128);

    uint64_t lane0 = hash::utils::bit_management::load64(input);
    uint64_t lane1 = hash::utils::bit_management::load64(input + 8);

    EXPECT_EQ(state[0], lane0 ^ 0xDEADBEEFDEADBEEF);
    EXPECT_EQ(state[1], lane1);
}

TEST_F(AbsorbTest, MixedFullAndPartialBlocks)
{
    uint8_t input[12];
    for (int i = 0; i < 12; i++) input[i] = i + 1;

    hash::utils::sponge::absorb(state, input, sizeof(input), 64);

    uint8_t temp[8] = {9, 10, 11, 12, 0x06, 0x00, 0x00, 0x80};
    uint64_t expected = hash::utils::bit_management::load64(temp);
    expected ^= hash::utils::bit_management::load64(input);

    EXPECT_EQ(state[0], expected);
}