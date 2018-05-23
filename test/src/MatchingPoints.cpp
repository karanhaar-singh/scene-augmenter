#include "gtest/gtest.h"

#include "MatchingPoints.hpp"


/**
 * Ensure construction fails on simple invalid inputs.
 */
TEST(simpleMatchingPoints, invalidInputs) {
    EXPECT_ANY_THROW(MatchingPoints(
            {{0, 0}}, {}));
    EXPECT_ANY_THROW(MatchingPoints(
            {}, {{0, 0}}));
}

/**
 * Ensure construction succeeds on simple valid inputs.
 */
TEST(simpleMatchingPoints, validInputs) {
    EXPECT_NO_THROW(MatchingPoints(
            {}, {}));
    EXPECT_NO_THROW(MatchingPoints(
            {{0, 0}}, {{0, 0}}));
}

/**
 * Ensure that an error is thrown if the Correspondences object references
 * invalid keypoint indices.
 */
TEST(typicalMatchingPoints, corresOutOfRange) {
    const std::vector<cv::Point> keypoints1{{0, 0}, {0, 1}};
    const std::vector<cv::Point> keypoints2{{0, 0}};

    EXPECT_ANY_THROW(MatchingPoints(keypoints1, keypoints2,
            {{0, 1}}));
    EXPECT_ANY_THROW(MatchingPoints(keypoints1, keypoints2,
            {{0, 0}, {1, 0}, {0, 1}}));
    EXPECT_ANY_THROW(MatchingPoints(keypoints1, keypoints2,
            {{1, 1}}));
}

/**
 * Ensure that no error is thrown if the Correspondences object and the
 * keypoints are internally consistent.
 */
TEST(typicalMatchingPoints, corresInRange) {
    const std::vector<cv::Point> keypoints1{{0, 0}, {0, 1}};
    const std::vector<cv::Point> keypoints2{{0, 0}};

    EXPECT_NO_THROW(MatchingPoints(keypoints1, keypoints2,
            {}));
    EXPECT_NO_THROW(MatchingPoints(keypoints1, keypoints2,
            {{0, 0}}));
    EXPECT_NO_THROW(MatchingPoints(keypoints1, keypoints2,
            {{1, 0}}));
    EXPECT_NO_THROW(MatchingPoints(keypoints1, keypoints2,
            {{0, 0}, {1, 0}}));
    EXPECT_NO_THROW(MatchingPoints(keypoints1, keypoints2,
            {{1, 0}, {0, 0}}));
}

