#include "../../include/hash_utils.h"

#include <gtest/gtest.h>

TEST(BitManagement, Rotl64)
{
    // Basic tests
    EXPECT_EQ(hash::utils::bit_management::rotl64(1, 2), 4);
    EXPECT_EQ(hash::utils::bit_management::rotl64(1, -1), 0x8000000000000000);

    // Real case tests
    uint64_t x = 0x0123456789ABCDEF;
    unsigned n = 8;
    EXPECT_EQ(hash::utils::bit_management::rotl64(x, n), 0x23456789ABCDEF01);

    // Edge cases
    n = 0;
    EXPECT_EQ(hash::utils::bit_management::rotl64(x, n), x);
    x = 0;
    n = 8;
    EXPECT_EQ(hash::utils::bit_management::rotl64(0, n), x);
    n = 10;
    EXPECT_EQ(hash::utils::bit_management::rotl64(0, n), x);
}

TEST(BitManagement, Load64)
{
    uint8_t bytes[8] = {0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};
    EXPECT_EQ(hash::utils::bit_management::load64(bytes), 0x0123456789ABCDEF);
}

TEST(BitManagement, Store64)
{
    uint64_t lane = 0x0123456789ABCDEF;
    uint8_t bytes[8] = {0};
    hash::utils::bit_management::store64(bytes, lane);
    uint8_t expected[8] = {0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};
    EXPECT_EQ(memcmp(bytes, expected, 8), 0);
}