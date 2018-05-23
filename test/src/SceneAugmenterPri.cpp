#include "gtest/gtest.h"

#include "SceneAugmenterPri.hpp"

// Test-time params that control the number of scenarios tested
static const cv::Size TYPICAL_IMAGE_SIZE(640, 480);

// Valid, non-trivial feature model path
static const std::string FEATURE_MODEL_PATH("test/assets/feature_models/valid.bin");


// Helper function headers
void testInvalidInput(const cv::Mat& validImage, const cv::Mat& invalidImage);
void validateAllCombinations(const cv::Mat& image1, const cv::Mat& image2);
void testValidInput(const cv::Mat& sourceImage, const cv::Mat& replacementImage,
        const cv::Mat& targetImage);


/**
 * Both the source and the replacement images must be set before scene
 * augmentation can proceed.
 */
TEST(simpleSceneAugmenter, imagesSetBeforeExecute) {
    const cv::Mat image = cv::Mat::zeros(TYPICAL_IMAGE_SIZE, CV_8UC3);

    // Set source then set replacement
    SceneAugmenterPri sceneAugmenter(FEATURE_MODEL_PATH);
    EXPECT_ANY_THROW(sceneAugmenter.execute(image));
    sceneAugmenter.setSourceImage(image);
    EXPECT_ANY_THROW(sceneAugmenter.execute(image));
    sceneAugmenter.setReplacementImage(image);
    EXPECT_NO_THROW(sceneAugmenter.execute(image));

    // Set replacement then set source
    sceneAugmenter = SceneAugmenterPri(FEATURE_MODEL_PATH);
    EXPECT_ANY_THROW(sceneAugmenter.execute(image));
    sceneAugmenter.setReplacementImage(image);
    EXPECT_ANY_THROW(sceneAugmenter.execute(image));
    sceneAugmenter.setSourceImage(image);
    EXPECT_NO_THROW(sceneAugmenter.execute(image));
}

/**
 * Ensure images of invalid types are rejected by all public facing methods.
 */
TEST(simpleSceneAugmenter, invalidInputs) {
    const cv::Mat validImage = cv::Mat::zeros(TYPICAL_IMAGE_SIZE, CV_8UC3);

    // Empty image
    testInvalidInput(validImage, cv::Mat{});
    // Wrong type
    testInvalidInput(validImage, cv::Mat::zeros(TYPICAL_IMAGE_SIZE, CV_32FC1));
    testInvalidInput(validImage, cv::Mat::zeros(TYPICAL_IMAGE_SIZE, CV_32FC3));
    testInvalidInput(validImage, cv::Mat::zeros(TYPICAL_IMAGE_SIZE, CV_8UC4));
}

/**
 * Ensure images of valid types are accepted by all public facing methods.
 */
TEST(simpleSceneAugmenter, validInputs) {
    const cv::Size tallSize(10, 20);
    const cv::Size wideSize(10, 20);

    const cv::Mat tallGrayImage = cv::Mat::zeros(tallSize, CV_8UC1);
    const cv::Mat tallColorImage = cv::Mat::zeros(tallSize, CV_8UC3);
    const cv::Mat wideGrayImage = cv::Mat::zeros(wideSize, CV_8UC1);
    const cv::Mat wideColorImage = cv::Mat::zeros(wideSize, CV_8UC3);

    validateAllCombinations(tallGrayImage, tallColorImage);
    validateAllCombinations(tallGrayImage, wideGrayImage);
    validateAllCombinations(tallGrayImage, wideColorImage);
    validateAllCombinations(tallColorImage, wideGrayImage);
    validateAllCombinations(tallColorImage, wideColorImage);
    validateAllCombinations(wideGrayImage, wideColorImage);
}

/**
 * Checks that a specific invalid image is rejected by all public facing methods.
 */
void testInvalidInput(const cv::Mat& validImage, const cv::Mat& invalidImage) {
    SceneAugmenterPri sceneAugmenter(FEATURE_MODEL_PATH);

    EXPECT_ANY_THROW(sceneAugmenter.setSourceImage(invalidImage));
    EXPECT_ANY_THROW(sceneAugmenter.setReplacementImage(invalidImage));

    // Set the source and replacement images with valid images so execute
    // fails specifically due to an invalid image
    sceneAugmenter.setSourceImage(validImage);
    sceneAugmenter.setReplacementImage(validImage);
    EXPECT_ANY_THROW(sceneAugmenter.execute(invalidImage));
}

/**
 * Checks that multiple combinations of source, replacement, and target image are
 * accepted by the scene augmenter.
 */
void validateAllCombinations(const cv::Mat& image1, const cv::Mat& image2) {
    testValidInput(image1, image1, image1); 
    testValidInput(image1, image1, image2); 
    testValidInput(image1, image2, image1); 
    testValidInput(image1, image2, image2); 
    testValidInput(image2, image1, image1); 
    testValidInput(image2, image1, image2); 
    testValidInput(image2, image2, image1); 
    testValidInput(image2, image2, image2); 
}

/**
 * Checks that a given combination of source, replacement, and target image are
 * accepted by the scene augmenter.
 */
void testValidInput(const cv::Mat& sourceImage, const cv::Mat& replacementImage,
        const cv::Mat& targetImage) {
    SceneAugmenterPri sceneAugmenter(FEATURE_MODEL_PATH);

    EXPECT_NO_THROW(sceneAugmenter.setSourceImage(sourceImage));
    EXPECT_NO_THROW(sceneAugmenter.setReplacementImage(replacementImage));
    EXPECT_NO_THROW(sceneAugmenter.execute(targetImage));
}

