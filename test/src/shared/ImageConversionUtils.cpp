#include "gtest/gtest.h"

#include <functional>
#include <unordered_set>

#include "opencv2/core.hpp"

#include "shared/ImageConversionUtils.hpp"

static const cv::Size TYPICAL_SIZE(640, 480);
static const std::unordered_set<int32_t> TYPICAL_CONV_MAT_TYPES{
        CV_8UC1, CV_8UC3, CV_32FC1, CV_32FC3};
static const std::unordered_set<int32_t> ALL_MAT_TYPES{
        CV_8UC1, CV_8UC2, CV_8UC3, CV_8UC4,
        CV_8SC1, CV_8SC2, CV_8SC3, CV_8SC4,
        CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
        CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
        CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
        CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
        CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};


// Helper function headers
void validateConversionOutput(const cv::Mat& inputImage,
        const std::function<cv::Mat(cv::Mat)> conversionFunction,
        const int32_t expectedOutputType);
void validateMat(const cv::Mat& image, const cv::Size& expectedSize,
        int32_t expectedType);


/**
 * Ensure that all conversion functions convert all valid inputs to their
 * respective proper cv::Mat types.
 */
TEST(simpleImageConversionUtils, validConversions) {
    for (int32_t cvType : TYPICAL_CONV_MAT_TYPES) {
        const cv::Mat zeroImage = cv::Mat::zeros(TYPICAL_SIZE, cvType);
        validateConversionOutput(zeroImage, shared::ImageConversionUtils::convertToGrayFloats, CV_32FC1);
        validateConversionOutput(zeroImage, shared::ImageConversionUtils::convertToColorFloats, CV_32FC3);
        validateConversionOutput(zeroImage, shared::ImageConversionUtils::convertToColorUint8, CV_8UC3);
    }
}

/**
 * Ensure that all conversion functions error out when they encounter
 * invalid cv::Mat types.
 */
TEST(simpleImageConversionUtils, invalidConversions) {
    for (int32_t cvType : ALL_MAT_TYPES) {
        if (TYPICAL_CONV_MAT_TYPES.count(cvType) == 0) {
            const cv::Mat zeroImage = cv::Mat::zeros(TYPICAL_SIZE, cvType);
            EXPECT_ANY_THROW(shared::ImageConversionUtils::convertToGrayFloats(zeroImage));
            EXPECT_ANY_THROW(shared::ImageConversionUtils::convertToColorFloats(zeroImage));
            EXPECT_ANY_THROW(shared::ImageConversionUtils::convertToColorUint8(zeroImage));
        }
    }
}

/**
 * Verifies that the output of a given function is of the expected type.
 */
void validateConversionOutput(const cv::Mat& inputImage,
        const std::function<cv::Mat(cv::Mat)> conversionFunction,
        const int32_t expectedOutputType) {
    const cv::Mat outputImage = conversionFunction(inputImage);
    validateMat(outputImage, inputImage.size(), expectedOutputType);
}

/**
 * Verifies that a cv::Mat has the desired properties.
 */
void validateMat(const cv::Mat& image, const cv::Size& expectedSize,
        int32_t expectedType) {
    EXPECT_EQ(image.size(), expectedSize); 
    EXPECT_EQ(image.type(), expectedType); 
}

