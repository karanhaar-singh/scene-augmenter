#include "gtest/gtest.h"

#include "core/FeatureExtractor.hpp"

// Test-time params that control the number of scenarios tested
static constexpr int32_t SIZE_OFFSET_START = -2;
static constexpr int32_t SIZE_OFFSET_END = 2;

// Valid, non-trivial feature model path
static const std::string FEATURE_MODEL_PATH("test/assets/feature_models/valid.bin");


/**
 * Ensure that algorithm runs to completion for a valid image.
 */
TEST(simpleFeatureExtractor, validImages) {
    core::FeatureExtractor featureExtractor(FEATURE_MODEL_PATH);
    const cv::Size templateSize = featureExtractor.getTemplateSize();
    const cv::Mat allZeros = cv::Mat::zeros(templateSize, CV_32FC1);

    EXPECT_NO_THROW(featureExtractor.execute(allZeros));
}

/**
 * Ensure that simple invalid input images throws exceptions.
 */
TEST(simpleFeatureExtractor, invalidImages) {
    core::FeatureExtractor featureExtractor(FEATURE_MODEL_PATH);
    const cv::Size templateSize = featureExtractor.getTemplateSize();

    // Wrong image types
    EXPECT_ANY_THROW(featureExtractor.execute(cv::Mat::zeros(templateSize, CV_32FC3)));
    EXPECT_ANY_THROW(featureExtractor.execute(cv::Mat::zeros(templateSize, CV_8UC1)));
    EXPECT_ANY_THROW(featureExtractor.execute(cv::Mat::zeros(templateSize, CV_8UC3)));

    // Wrong image dimensions
    for (int32_t dWidth = SIZE_OFFSET_START; dWidth < SIZE_OFFSET_END; dWidth++) {
        for (int32_t dHeight = SIZE_OFFSET_START; dHeight < SIZE_OFFSET_END; dHeight++) {
            const cv::Size testSize(templateSize.width + dWidth,
                    templateSize.height + dHeight);
            if (testSize != templateSize) {
                const cv::Mat allZeros = cv::Mat::zeros(testSize, CV_32FC1);
                EXPECT_ANY_THROW(featureExtractor.execute(allZeros));
            }
        }
    }
}

