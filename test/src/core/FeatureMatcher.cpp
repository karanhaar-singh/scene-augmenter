#include "gtest/gtest.h"

#include "core/Definitions.hpp"
#include "core/FeatureMatcher.hpp"

// Test-time params that control the number of scenarios tested
static constexpr uint32_t BIT_CHUNK_SIZE = 8u;

// Useful common BRIEF features
static const std::string ALL_ZEROS(core::NUM_BRIEF_BITS, '0');
static const std::string ALL_ONES(core::NUM_BRIEF_BITS, '1');
static const core::FeatureVector ALL_ZERO_BITS(ALL_ZEROS);
static const core::FeatureVector ALL_ONE_BITS(ALL_ONES);


// Helper function headers
core::FeatureVector buildBitvector(uint32_t numSetBits);


/**
 * Simple test for correctness when both feature vectors are identical.
 * We vary the number of set bits in both feature vector.
 */
TEST(simpleFeatureMatcher, allFeaturesSame) {
    // Check preconditions of constants
    ASSERT_GT(core::NUM_BRIEF_BITS, 0u);

    // Varying the number of set bits in all feature vectors
    for (uint32_t numSetBits = 0; numSetBits < core::NUM_BRIEF_BITS; numSetBits++) {
        const core::FeatureVector featureVector = buildBitvector(numSetBits);
        const uint32_t matchScore = core::FeatureMatcher::execute(
                featureVector, featureVector);
        EXPECT_EQ(matchScore, 0u);
    }
}

/**
 * Simple test for correctness when the first feature vector is all zeros
 * We vary the number of set bits in the second feature vector.
 */
TEST(simpleFeatureMatcher, firstFeatureZeros) {
    // Check preconditions of constants
    ASSERT_GT(core::NUM_BRIEF_BITS, 0u);

    // Varying the number of set bits in all feature vectors
    for (uint32_t numSetBits = 0; numSetBits < core::NUM_BRIEF_BITS; numSetBits++) {
        const core::FeatureVector featureVector = buildBitvector(numSetBits);
        const uint32_t matchScore = core::FeatureMatcher::execute(
                ALL_ZERO_BITS, featureVector);
        EXPECT_EQ(matchScore, numSetBits);
    }
}

/**
 * Builds a feature vector with the specified number of bits set clamped by the max
 * number of bits the bitset can hold.
 */
core::FeatureVector buildBitvector(uint32_t numSetBits) {
    uint32_t clampedNumSetBits = std::min(numSetBits, core::NUM_BRIEF_BITS);
    core::FeatureVector bitvector(ALL_ONES, 0, clampedNumSetBits);
    return bitvector;
}

