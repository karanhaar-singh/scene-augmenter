#include "gtest/gtest.h"

#include "core/Definitions.hpp"

#include "CorrespondenceFinder.hpp"

// Test-time params that control the number of scenarios tested
static constexpr uint32_t TYPICAL_MIN_TOP_DISTANCE = 15u;
static constexpr uint32_t MAX_FEATURE_VECS = 4u;
static constexpr uint32_t NUM_SET_BITS_HOP = core::NUM_BRIEF_BITS/MAX_FEATURE_VECS;
static constexpr uint32_t BIT_CHUNK_SIZE = 8u;

// Useful common BRIEF features
static const std::string ALL_ZEROS(core::NUM_BRIEF_BITS, '0');
static const std::string ALL_ONES(core::NUM_BRIEF_BITS, '1');
static const core::FeatureVector ALL_ZERO_BITS(ALL_ZEROS);
static const core::FeatureVector ALL_ONE_BITS(ALL_ONES);


// Helper function headers
void testAllFeaturesSame(const CorrespondenceFinder& correspondenceFinder,
        const core::FeatureVector& featureValue);
void verifyCorrectMatches(const CorrespondenceFinder& correspondenceFinder,
        const std::vector<core::FeatureVector>& briefFeatures1,
        const std::vector<core::FeatureVector>& briefFeatures2,
        const Correspondences& trueMatches);
std::vector<core::FeatureVector> buildSlidingBitvectors(uint32_t bitChunkSize);
core::FeatureVector buildFeatureVector(uint32_t numSetBits);


/**
 * Ensure no matches are returned if either input is too small.
 */
TEST(simpleCorrespondenceFinder, tooFewPoints) {
    const CorrespondenceFinder correspondenceFinder(TYPICAL_MIN_TOP_DISTANCE);
    
    // first input: 0 pts, second input: 0 pts
    verifyCorrectMatches(correspondenceFinder, {}, {}, {});
    // first input: 1 pt, second input: 0 pts
    verifyCorrectMatches(correspondenceFinder, {core::FeatureVector{}}, {}, {});
    // first input: 0 pts, second input: 1 pt
    verifyCorrectMatches(correspondenceFinder,
            {core::FeatureVector{}},
            {core::FeatureVector{}}, {});
    // first input: 1 pts, second input: 1 pt
    verifyCorrectMatches(correspondenceFinder, {}, {core::FeatureVector{}}, {});

    // first input: 0 pts, second input: 2 pt
    verifyCorrectMatches(correspondenceFinder, {},
            {core::FeatureVector{}, core::FeatureVector{}}, {});
    // first input: 2 pts, second input: 0 pt
    verifyCorrectMatches(correspondenceFinder,
            {core::FeatureVector{}, core::FeatureVector{}}, {}, {});
}

/**
 * Ensure match is returned with the minimum valid input.
 */
TEST(simpleCorrespondenceFinder, minRequiredPoints) {
    const CorrespondenceFinder correspondenceFinder(TYPICAL_MIN_TOP_DISTANCE);
    
    // first input: 1 pts, second input: 2 pt
    // satisfies minTopDistance requirement to match
    verifyCorrectMatches(correspondenceFinder,
            {core::FeatureVector(ALL_ZERO_BITS)},
            {core::FeatureVector(ALL_ZERO_BITS),
                    core::FeatureVector(ALL_ONE_BITS)},
            {{0, 0}});
}

/**
 * Simple test for correctness when all feature vectors in both input vectors
 * are identical.  We (universally) vary the number of set bits in each feature
 * vector and the number of feature vectors in each input vector.
 */
TEST(typicalCorrespondenceFinder, allFeaturesSame) {
    // Check preconditions of constants
    ASSERT_EQ(core::NUM_BRIEF_BITS % MAX_FEATURE_VECS, 0u);

    const CorrespondenceFinder correspondenceFinder(TYPICAL_MIN_TOP_DISTANCE);
    
    // Varying the number of set bits in all feature vectors
    for (uint32_t numSetBits = 0; numSetBits < core::NUM_BRIEF_BITS;
            numSetBits+= NUM_SET_BITS_HOP) {
        const core::FeatureVector featureValue = buildFeatureVector(numSetBits);
        testAllFeaturesSame(correspondenceFinder, featureValue);
    }
}

/**
 * Simple test for correctness when all of the feature vectors in the first input
 * are expected to be matched to the very first element of the second input.
 * The minTopDistance requirement to match is met.
 */
TEST(typicalCorrespondenceFinder, sameTargetPoint) {
    // Check preconditions of constants
    ASSERT_GE(MAX_FEATURE_VECS, 2u);

    const CorrespondenceFinder correspondenceFinder(TYPICAL_MIN_TOP_DISTANCE);
    
    for (uint32_t size1 = 0; size1 < MAX_FEATURE_VECS; size1++) {
        // All feature vectors in first input are zero bitvectors
        std::vector<core::FeatureVector> briefFeatures1(size1, ALL_ZERO_BITS);

        // Matches should map each feature vector in the first input to the first
        // element in the second input
        Correspondences trueMatches;
        for (uint32_t iMatch = 0; iMatch < size1; iMatch++) {
            trueMatches.emplace_back(iMatch, 0);
        }

        for (uint32_t size2 = 2; size2 < MAX_FEATURE_VECS; size2++) {
            // All feature vectors in second input are one bitvectors except for the first
            // element, which is the zero bitvector
            std::vector<core::FeatureVector> briefFeatures2(size2, ALL_ONE_BITS);
            briefFeatures2[0] = ALL_ZERO_BITS;

            verifyCorrectMatches(correspondenceFinder,
                    briefFeatures1, briefFeatures2, trueMatches);
        }
    }
}

/**
 * Simple test for correctness when all of the feature vectors in the first input
 * are expected to be matched to the same index in the second input.  The
 * minTopDistance param to the matcher is varied.
 */
TEST(typicalCorrespondenceFinder, diffTargetPoints) {
    // Check preconditions of constants
    ASSERT_GE(MAX_FEATURE_VECS, 2u);
    ASSERT_GT(BIT_CHUNK_SIZE, 0u);
    ASSERT_LE(BIT_CHUNK_SIZE, core::NUM_BRIEF_BITS);
    ASSERT_EQ(core::NUM_BRIEF_BITS % BIT_CHUNK_SIZE, 0u);
    
    // Both input vectors are the same
    std::vector<core::FeatureVector> briefFeatures1 = buildSlidingBitvectors(BIT_CHUNK_SIZE);
    std::vector<core::FeatureVector> briefFeatures2 = buildSlidingBitvectors(BIT_CHUNK_SIZE);

    // Matches should map each feature vector in the first input to the
    // element in the second input of the same index
    Correspondences trueMatches;
    for (uint32_t iMatch = 0; iMatch < briefFeatures1.size(); iMatch++) {
        trueMatches.emplace_back(iMatch, iMatch);
    }

    // Vary minTopDist param to matcher
    for (uint32_t minTopDist = 0; minTopDist <= core::NUM_BRIEF_BITS; minTopDist++) {
        Correspondences currTrueMatches =
                minTopDist <= BIT_CHUNK_SIZE ? trueMatches :
                Correspondences{};
        verifyCorrectMatches(CorrespondenceFinder(minTopDist), briefFeatures1, briefFeatures2,
                currTrueMatches);
    }
}



/**
 * Test multiple numbers of (identical) feature vectors for a given featureValue.
 * The number of feature vectors in both input vectors are independently varied.
 * We expect no matches to be returned due to the min top distance constraint.
 */
void testAllFeaturesSame(const CorrespondenceFinder& correspondenceFinder,
        const core::FeatureVector& featureValue) {
    for (uint32_t size1 = 0; size1 < MAX_FEATURE_VECS; size1++) {
        for (uint32_t size2 = 0; size2 < MAX_FEATURE_VECS; size2++) {
            const std::vector<core::FeatureVector> briefFeatures1(size1, featureValue);
            const std::vector<core::FeatureVector> briefFeatures2(size2, featureValue);
            verifyCorrectMatches(correspondenceFinder, briefFeatures1, briefFeatures2,
                    Correspondences{});
        }
    }
}

/**
 * Tests that running BRIEF matching on the specified inputs gives the specified
 * output.
 */
void verifyCorrectMatches(const CorrespondenceFinder& correspondenceFinder,
        const std::vector<core::FeatureVector>& briefFeatures1,
        const std::vector<core::FeatureVector>& briefFeatures2,
        const Correspondences& trueMatches) {
    Correspondences computedMatches =
            correspondenceFinder.execute(briefFeatures1, briefFeatures2);
    EXPECT_EQ(computedMatches, trueMatches);
}

/**
 * Builds a vector of feature vectors.  The first feature vector is the zero bitvector.
 * Each subsequent feature vector has bitChunkSize more bits set than the previous feature
 * vector.
 */
std::vector<core::FeatureVector> buildSlidingBitvectors(uint32_t bitChunkSize) {
    std::vector<core::FeatureVector> slidingBitvectors;
    for (uint32_t numSetBits = 0; numSetBits <= core::NUM_BRIEF_BITS;
            numSetBits += bitChunkSize) {
        const core::FeatureVector currBitvector = buildFeatureVector(numSetBits);
        slidingBitvectors.push_back(currBitvector);
    }

    return slidingBitvectors;
}

/**
 * Builds a feature vector with the specified number of bits set clamped by the max
 * number of bits the bitset can hold.
 */
core::FeatureVector buildFeatureVector(uint32_t numSetBits) {
    uint32_t clampedNumSetBits = std::min(numSetBits, core::NUM_BRIEF_BITS);
    core::FeatureVector bitvector(ALL_ONES, 0, clampedNumSetBits);
    return bitvector;
}

