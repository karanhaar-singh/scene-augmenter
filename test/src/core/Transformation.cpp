#include "gtest/gtest.h"

#include "opencv2/core.hpp"

#include "core/homography/Definitions.hpp"
#include "core/Definitions.hpp"
#include "core/Transformation.hpp"

// Test-time params that control the number of scenarios tested
static constexpr uint32_t NUM_BUILD_POINTS = core::homography::MIN_BUILD_POINTS;
static constexpr uint32_t MAX_NUM_POINTS = NUM_BUILD_POINTS + 5;


// Helper function headers
void validateFailure(const std::vector<cv::Point>& fromPoints,
        const std::vector<cv::Point>& toPoints);
void validateSuccess(const std::vector<cv::Point>& fromPoints,
        const std::vector<cv::Point>& toPoints);


/**
 * Verifies that the algorithm errors out with inputs of wrong sizes.
 */
TEST(simpleTransformation, wrongNumBuildPoints) {
    core::Transformation transformation;

    std::vector<cv::Point> points;
    for (uint32_t numPoints = 0; numPoints <= MAX_NUM_POINTS; numPoints++) {
        if (numPoints != NUM_BUILD_POINTS) {
            EXPECT_ANY_THROW(transformation.build(points, points));
        }
        points.emplace_back(0, 0);
    }
}

/**
 * Verifies that the core::Transformation object is set to an
 * invalid state if the inputs have the correct number of
 * points, but are in a degenerate configuration.
 */
TEST(simpleTransformation, pointsBadForEstimation) {
    ASSERT_EQ(NUM_BUILD_POINTS, 4u);

    // At least two points are identical
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {0, 0}, {0, 0}, {0, 0}});
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {0, 0}, {0, 0}, {0, 1}});
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {0, 0}, {0, 1}, {1, 1}});

    // At least three points are collinear
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {0, 1}, {0, 2}, {1, 1}});
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {1, 0}, {2, 0}, {1, 1}});
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}});
    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}});
}

/**
 * Verifies that the core::Transformation object is set to an
 * invalid state if the inputs have the correct number of
 * points, but are not in a degenerate configuration, but don't
 * have consistent orientations.
 */
TEST(simpleTransformation, orientationsDoNotMatch) {
    ASSERT_EQ(NUM_BUILD_POINTS, 4u);

    validateFailure(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}});
}

/**
 * Verifies that the core::Transformation object is set to an
 * valid state for various typical valid transformations.
 */
TEST(simpleTransformation, simpleTransforms) {
    ASSERT_EQ(NUM_BUILD_POINTS, 4u);

    // Identity
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}});
    // Scale (isotropic and non-isotropic)
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {2, 0}, {0, 2}, {2, 2}});
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 0}, {2, 0}, {0, 3}, {2, 3}});
    // Translation (1 and 2 coordinates)
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 1}, {1, 1}, {0, 2}, {1, 2}});
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 1}, {2, 1}, {1, 2}, {2, 2}});
    // Rotation
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 0}, {1, 1}, {0, 0}, {0, 1}});
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{0, 1}, {0, 0}, {1, 1}, {1, 0}});
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 1}, {0, 1}, {1, 0}, {0, 0}});
    // Scale + translation
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 1}, {3, 1}, {1, 3}, {3, 3}});
    // Affine
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 1}, {3, 1}, {4, 4}, {6, 4}});
    // Perspective
    validateSuccess(
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
            {{1, 1}, {3, 1}, {1, 3}, {5, 5}});
}

/**
 * Verifies that the given matching points produces an invalid
 * core::Transformation object and such object behaves properly.
 */
void validateFailure(const std::vector<cv::Point>& fromPoints,
        const std::vector<cv::Point>& toPoints) {
    core::Transformation transformation;
    transformation.build(fromPoints, toPoints);
    EXPECT_FALSE(transformation.isValid());

    const std::vector<cv::Point> transformedFromPoints =
            transformation.apply(fromPoints);
    EXPECT_EQ(transformedFromPoints, std::vector<cv::Point>{});
}

/**
 * Verifies that the given matching points produces a valid
 * core::Transformation object and such object behaves properly.
 */
void validateSuccess(const std::vector<cv::Point>& fromPoints,
        const std::vector<cv::Point>& toPoints) {
    // Build transformation and validate self-transform
    core::Transformation transformation;
    transformation.build(fromPoints, toPoints);
    EXPECT_TRUE(transformation.isValid());

    const std::vector<cv::Point> transformedFromPoints =
            transformation.apply(fromPoints);
    EXPECT_EQ(transformedFromPoints, toPoints);
}

