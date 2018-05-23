#include "SceneAugmenterPri.hpp"

#include "opencv2/imgproc.hpp"

#include "shared/ImageConversionUtils.hpp"

#include "core/Transformation.hpp"

void SceneAugmenterPri::setSourceImage(const cv::Mat& newSourceImage) {
    validateImage(newSourceImage);
    sourceImageDescription = buildImageDescription(newSourceImage);
}

void SceneAugmenterPri::setReplacementImage(const cv::Mat& newReplacementImage) {
    validateImage(newReplacementImage);
    replacementImageFloat = shared::ImageConversionUtils::convertToColorFloats(newReplacementImage);
}

/**
 * Algorithm: Same pipeline as described in the README
 */
cv::Mat SceneAugmenterPri::execute(const cv::Mat& targetImage) const {
    validateImage(targetImage);
    shared::VALIDATE_ARGUMENT(sourceImageDescription.size.area() > 0,
            "SceneAugmenter: Source image is not set");
    shared::VALIDATE_ARGUMENT(!replacementImageFloat.empty(),
            "SceneAugmenter: Replacement image is not set");

    const ImageDescription targetImageDescription = buildImageDescription(targetImage);

    const Correspondences correspondences = correspondenceFinder.execute(
            sourceImageDescription.featureVectors, targetImageDescription.featureVectors);
    const MatchingPoints matchingPoints(sourceImageDescription.keypoints,
            targetImageDescription.keypoints, correspondences);
    const core::Transformation transformation = transformationFitter.execute(matchingPoints);
    const cv::Mat augmentedImage = augment(targetImage, transformation);

    return augmentedImage;
}

void SceneAugmenterPri::validateImage(const cv::Mat& image) const {
    shared::VALIDATE_ARGUMENT(!image.empty(), "SceneAugmenter: Image is empty");
    shared::VALIDATE_ARGUMENT(image.type() == CV_8UC1 || image.type() == CV_8UC3,
            "SceneAugmenter: Image must be 1 or 3 channels and have data of type uint8");
}

SceneAugmenterPri::ImageDescription SceneAugmenterPri::buildImageDescription(
        const cv::Mat& imageToDescribe) const {
    // Internally, we perform operations on grayscale float32-pixel images
    const cv::Mat image = shared::ImageConversionUtils::convertToGrayFloats(imageToDescribe);

    const std::vector<cv::Point> keypoints = keypointDetector.execute(image);
    const std::vector<core::FeatureVector> featureVectors =
            sceneFeatureExtractor.execute(image, keypoints);

    const ImageDescription imageDescription(image.size(), keypoints, featureVectors);
    return imageDescription;
}

cv::Mat SceneAugmenterPri::augment(const cv::Mat& targetImage,
        const core::Transformation& transformation) const {
    // Skew the replacement image so it will fit exactly on to the found source object
    cv::Mat scaledReplacementImageFloat;
    cv::resize(replacementImageFloat, scaledReplacementImageFloat, sourceImageDescription.size);

    // Perform augmentation in the color float32 space
    const cv::Mat targetImageFloat = shared::ImageConversionUtils::convertToColorFloats(targetImage);
    const cv::Mat augmentedImageFloat = transformation.augment(
            targetImageFloat, scaledReplacementImageFloat);
    const cv::Mat augmentedImage = shared::ImageConversionUtils::convertToColorUint8(augmentedImageFloat);

    return augmentedImage;
}

