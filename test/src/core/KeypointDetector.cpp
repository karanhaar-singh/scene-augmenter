#include "gtest/gtest.h"

#include "core/Definitions.hpp"
#include "core/KeypointDetector.hpp"

// Test-time params that control the number of scenarios tested
static constexpr int32_t TYPICAL_SIDE = 15;
static constexpr int32_t MAX_DIM_SCALE = 3;
static constexpr int32_t BOX_DIM_SCALE_MULT = 3;

static const cv::Size typicalSize(TYPICAL_SIDE, TYPICAL_SIDE);


// Helper function headers
void validateNumPoints(const core::KeypointDetector& keypointDetector,
        const cv::Mat& inputImage, uint32_t numPoints);


/**
 * Ensure that algorithm runs to completion for a valid image.
 */
TEST(simpleKeypointDetector, validImages) {
    const core::KeypointDetector keypointDetector;
    const cv::Mat allZeros = cv::Mat::zeros(typicalSize, CV_32FC1);

    EXPECT_NO_THROW(keypointDetector.execute(allZeros));
}

/**
 * Ensure that simple invalid input images throws exceptions.
 */
TEST(simpleKeypointDetector, invalidImages) {
    const core::KeypointDetector keypointDetector;

    // Empty image
    EXPECT_ANY_THROW(keypointDetector.execute(cv::Mat{}));

    // Wrong image types
    EXPECT_ANY_THROW(keypointDetector.execute(cv::Mat::zeros(typicalSize, CV_32FC3)));
    EXPECT_ANY_THROW(keypointDetector.execute(cv::Mat::zeros(typicalSize, CV_8UC1)));
    EXPECT_ANY_THROW(keypointDetector.execute(cv::Mat::zeros(typicalSize, CV_8UC3)));
}

/** 
 * Ensure correctness on blank images, each with differing image dims.
 */
TEST(typicalImagesKeypointDetector, blankImages) {
    const core::KeypointDetector keypointDetector;

    for (int32_t rowScale = 1; rowScale <= MAX_DIM_SCALE; rowScale++) {
        for (int32_t colScale = 1; colScale <= MAX_DIM_SCALE; colScale++) {
            // Simple blank images with no corners, we test both a black and a white image
            const cv::Mat zerosImage = cv::Mat::zeros(
                    rowScale*TYPICAL_SIDE, colScale*TYPICAL_SIDE, CV_32FC1);
            validateNumPoints(keypointDetector, zerosImage, 0u);

            const cv::Mat onesImage = cv::Mat::ones(
                    rowScale*TYPICAL_SIDE, colScale*TYPICAL_SIDE, CV_32FC1);
            validateNumPoints(keypointDetector, onesImage, 0u);
        }
    }

}

/** 
 * Ensure correctness on images with a dot, each with differing image dims.
 */
TEST(typicalImagesKeypointDetector, imagesWithDots) {
    const core::KeypointDetector keypointDetector;

    for (int32_t rowScale = 1; rowScale <= MAX_DIM_SCALE; rowScale++) {
        for (int32_t colScale = 1; colScale <= MAX_DIM_SCALE; colScale++) {
            // Draw a dot in the middle of the image
            cv::Mat dotInCenter = cv::Mat::zeros(rowScale*TYPICAL_SIDE,
                    colScale*TYPICAL_SIDE, CV_32FC1);
            dotInCenter.at<float>(dotInCenter.rows/2, dotInCenter.cols/2) = 1.0f;

            // Test image and image with colors inverted
            validateNumPoints(keypointDetector, dotInCenter, 1u);
            validateNumPoints(keypointDetector, 1.0f - dotInCenter, 1u);
        }
    }

}

/** 
 * Ensure correctness on images with lines but no corners, each with differing image dims.
 */
TEST(typicalImagesKeypointDetector, imagesWithLines) {
    // Check preconditions of constants
    ASSERT_GE(BOX_DIM_SCALE_MULT, 2);

    const core::KeypointDetector keypointDetector;

    for (int32_t rowScale = 1; rowScale <= MAX_DIM_SCALE; rowScale++) {
        for (int32_t colScale = 1; colScale <= MAX_DIM_SCALE; colScale++) {
            // Draw a rectangle extending to the boundaries of the image. The result image
            // has two edge lines where the colors flip but no corner points.  Edge suppression
            // should ensure we get no keypoints
            cv::Mat edgeLines = cv::Mat::zeros(rowScale*BOX_DIM_SCALE_MULT*TYPICAL_SIDE,
                    colScale*BOX_DIM_SCALE_MULT*TYPICAL_SIDE, CV_32FC1);
            // Draw aforementioned rectangle
            const cv::Rect boxRoi(0, edgeLines.rows/3, edgeLines.cols, edgeLines.rows/3);
            edgeLines(boxRoi).setTo(cv::Scalar(1.0f));

            // Test image, image with colors inverted, and their transposes, which
            // verifies that we suppress edges in both the X and Y directions
            validateNumPoints(keypointDetector, edgeLines, 0u);
            validateNumPoints(keypointDetector, 1.0f - edgeLines, 0u);
            validateNumPoints(keypointDetector, edgeLines.t(), 0u);
            validateNumPoints(keypointDetector, (1.0f - edgeLines).t(), 0u);
        }
    }

}

/**
 * Verify that running algorithm on the specified image has at least the specified
 * number of points.
 */
void validateNumPoints(const core::KeypointDetector& keypointDetector,
        const cv::Mat& inputImage, uint32_t numPoints) {
    const std::vector<cv::Point> keypoints = keypointDetector.execute(inputImage);
    EXPECT_GE(keypoints.size(), numPoints);
}

