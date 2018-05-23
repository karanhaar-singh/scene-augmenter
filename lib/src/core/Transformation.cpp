#include "core/Transformation.hpp"

#include <cmath>

#include "opencv2/imgproc.hpp"

#include "shared/Definitions.hpp"

#include "core/homography/Builder.hpp"
#include "core/homography/Definitions.hpp"
#include "core/homography/Evaluator.hpp"
#include "core/homography/SanityChecker.hpp"

namespace core {

void Transformation::build(const std::vector<cv::Point>& fromPts,
        const std::vector<cv::Point>& toPts) {
    shared::VALIDATE_ARGUMENT(fromPts.size() == homography::MIN_BUILD_POINTS,
            "core::Transformation::build takes in 4 points per param");
    shared::VALIDATE_ARGUMENT(toPts.size() == homography::MIN_BUILD_POINTS,
            "core::Transformation::build takes in 4 points per param");

    isTransformationValid = false;

    // First, check sanity of the matches
    const bool buildMatchesSane = homography::SanityChecker::areMatchesSane(fromPts, toPts);
    if (!buildMatchesSane) {
        return;
    }

    // Build the homography only if the sanity checks pass
    const cv::Mat homography = homography::Builder::build(fromPts, toPts);

    transformation = homography;
    isTransformationValid = true;
}

bool Transformation::isValid() const {
    return isTransformationValid;
}

std::vector<cv::Point> Transformation::apply(const std::vector<cv::Point>& inputPts) const {
    if (!isTransformationValid) {
        return std::vector<cv::Point>{};
    }

    const std::vector<cv::Point> outputPoints =
            homography::Evaluator::applyPoints(inputPts, transformation);

    return outputPoints;
}

cv::Mat Transformation::augment(const cv::Mat& targetImage,
        const cv::Mat& replacementImage) const {
    if (!isTransformationValid) {
        return targetImage.clone();
    }

    // Check sanity of the proposed augmentation by first transforming the image corners
    const std::vector<cv::Point> replacementCorners{{0, 0}, {replacementImage.cols-1, 0},
            {replacementImage.cols-1, replacementImage.rows-1}, {0, replacementImage.rows-1}};
    const cv::Rect targetRegion(0, 0, targetImage.cols, targetImage.rows);
    const bool augmentationIsSane = homography::Evaluator::isAugmentationSane(
            replacementCorners, targetRegion, transformation);
    if (!augmentationIsSane) {
        return targetImage.clone();
    }

    // We can proceed with the augmentation
    const cv::Mat augmentedImage = homography::Evaluator::augment(
            targetImage, replacementImage, transformation);

    return augmentedImage;
}

}

