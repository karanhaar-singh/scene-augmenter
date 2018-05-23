#include "gtest/gtest.h"

#include "TransformationFitter.hpp"

// Helper function headers
bool isTransformationValid(const TransformationFitter& transformationFitter,
        const std::vector<cv::Point>& fromPts, const std::vector<cv::Point>& toPts);


/**
 * Ensures output core::Transformation is detected as invalid when
 * not enough matching points are passed in.
 */
TEST(simpleTransformationFitter, tooFewMatchingPoints) {
    const TransformationFitter transformationFitter;

    EXPECT_FALSE(isTransformationValid(transformationFitter, {}, {}));
    EXPECT_FALSE(isTransformationValid(transformationFitter,
            {{0, 0}}, {{0, 0}}));
    EXPECT_FALSE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}}));
}

/**
 * Ensures output core::Transformation is detected as valid when
 * enough valid matching points are passed in.
 */
TEST(simpleTransformationFitter, minNumMatchingPoints) {
    const TransformationFitter transformationFitter;

    EXPECT_TRUE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 1}, {1, 2}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 1}, {1, 2}}));
}

/**
 * Ensures output core::Transformation is detected as invalid when
 * a sufficient number of matching points are passed in, but form 
 * a degenerate configuration in some way.
 */
TEST(typicalTransformationFitter, degeneratePoints) {
    const TransformationFitter transformationFitter;

    EXPECT_FALSE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
            {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}));
    EXPECT_FALSE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
            {{0, 0}, {0, 1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}));
    EXPECT_FALSE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 1}, {1, 0}, {0, 0}, {0, 0}, {0, 0}},
            {{0, 0}, {0, 1}, {1, 0}, {0, 0}, {0, 0}, {0, 0}}));
}

/**
 * Ensures output core::Transformation is detected as invalid when
 * the input points are too noisy to produce enough inliers.
 */
TEST(typicalTransformationFitter, notEnoughInliers) {
    const TransformationFitter transformationFitter;

    EXPECT_FALSE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 10}, {10, 0}, {10, 10}, {0, 0}},
            {{0, 0}, {0, 10}, {10, 0}, {10, 1}, {10, 10}}));
}

/**
 * Ensures output core::Transformation is detected as valid when
 * the input points produce enough inliers.
 */
TEST(typicalTransformationFitter, enoughInliers) {
    const TransformationFitter transformationFitter;

    EXPECT_TRUE(isTransformationValid(transformationFitter,
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 1}, {1, 2}, {2, 2}, {3, 3}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 1}, {1, 2}, {2, 2}, {13, 13}}));
}

/**
 * Helper function that builds a core::Transformation object and checks for validity.
 */
bool isTransformationValid(const TransformationFitter& transformationFitter,
        const std::vector<cv::Point>& fromPts, const std::vector<cv::Point>& toPts) {
    const MatchingPoints matches(fromPts, toPts);
    core::Transformation transformation = transformationFitter.execute(matches);

    return transformation.isValid();
}

