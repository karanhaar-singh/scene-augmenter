#include "gtest/gtest.h"

#include "core/Definitions.hpp"

#include "SceneFeatureExtractor.hpp"

// Test-time params that control the number of scenarios tested
static const cv::Size TYPICAL_IMAGE_SIZE(640, 480);
static constexpr uint32_t MAX_NUM_KEYPOINTS = 10u;

// Valid, non-trivial feature model path
static const std::string FEATURE_MODEL_PATH("test/assets/feature_models/valid.bin");


/**
 * Ensures algorithm runs for simple valid inputs.
 */
TEST(simpleSceneFeatureExtractor, sanity) {
    const SceneFeatureExtractor sceneFeatureExtractor(FEATURE_MODEL_PATH);
    const cv::Mat image = cv::Mat::zeros(cv::Size(2, 2), CV_32FC1);

    EXPECT_NO_THROW(sceneFeatureExtractor.execute(image,
            {{0, 0}, {0, 1}, {1, 0}, {1, 1}}));
}

/**
 * Ensures algorithm errors out when keypoints are out of
 * range of the image.
 */
TEST(simpleSceneFeatureExtractor, invalidKeypointShifts) {
    const SceneFeatureExtractor sceneFeatureExtractor(FEATURE_MODEL_PATH);
    const cv::Mat image = cv::Mat::zeros(cv::Size(2, 2), CV_32FC1);

    const std::vector<cv::Point> badKeypoints{
            {-1, 0}, {2, 0}, {0, -1}, {0, 2},
            {-1, -1}, {2, 2}, {2, -1}, {-1, 2}};
    for (const cv::Point& badKeypoint : badKeypoints) {
        EXPECT_ANY_THROW(sceneFeatureExtractor.execute(
                image, {badKeypoint}));
    }
}

/**
 * Verifies that the number of extracted feature vectors is the same as
 * the number of image keypoints.
 */
TEST(simpleSceneFeatureExtractor, validateNumFeatureVectors) {
    const SceneFeatureExtractor sceneFeatureExtractor(FEATURE_MODEL_PATH);
    const cv::Mat image = cv::Mat::zeros(TYPICAL_IMAGE_SIZE, CV_32FC1);
    const cv::Point centerKeypoint(
            (TYPICAL_IMAGE_SIZE.width - 1)/2,
            (TYPICAL_IMAGE_SIZE.height - 1)/2);

    for (uint32_t numKeypoints = 0; numKeypoints <= MAX_NUM_KEYPOINTS; numKeypoints++) {
        const std::vector<cv::Point> keypoints(numKeypoints, centerKeypoint);
        const std::vector<core::FeatureVector> featureVectors =
                sceneFeatureExtractor.execute(image, keypoints);
        EXPECT_EQ(featureVectors.size(), numKeypoints);
    }
}

