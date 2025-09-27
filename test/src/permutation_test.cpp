#include "../../include/hash_utils.h"

#include <gtest/gtest.h>

TEST(PermutationTests, theta)
{
    uint64_t state[25] = {0};
    EXPECT_EQ(hash::utils::permutations::permutations_helpers::parities(state)[0], 0);

    for(uint64_t i=0 ; i<25 ; i++)
    {
        state[i] = i;
    }

    EXPECT_EQ(hash::utils::permutations::permutations_helpers::parities(state)[1], 9);

    hash::utils::permutations::theta(state);

    EXPECT_EQ(state[0], 26);
    EXPECT_EQ(state[5], 31);
}
