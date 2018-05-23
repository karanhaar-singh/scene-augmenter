#include "core/homography/Evaluator.hpp"

#include <cmath>

#include "opencv2/imgproc.hpp"

#include "shared/Definitions.hpp"

#include "core/homography/Definitions.hpp"
#include "core/homography/SanityChecker.hpp"

namespace core {
namespace homography {

std::vector<cv::Point> Evaluator::applyPoints(
        const std::vector<cv::Point>& inputPts, const cv::Mat& homography) {
    if (inputPts.size() == 0) {
        return std::vector<cv::Point>{};
    }

    const cv::Mat inputMat = convertPointsToMat(inputPts);
    const cv::Mat outputMat = inputMat*homography.t();
    const std::vector<cv::Point> outputPoints = convertMatToPoints(outputMat);

    return outputPoints;
}

bool Evaluator::isAugmentationSane(const std::vector<cv::Point>& replacementPoints,
        const cv::Rect& targetRegion, const cv::Mat& homography) {
    const std::vector<cv::Point> mappedPoints = applyPoints(replacementPoints, homography); 

    // First, make sure the mapping between the original and transformed points are sane
    const bool matchesSane = SanityChecker::areMatchesSane(replacementPoints, mappedPoints);
    if (matchesSane) {
        // Next, validate that all transformed points lie within the target region
        const bool targetContainsMappedPoints = arePointsInside(mappedPoints, targetRegion);
        return targetContainsMappedPoints;
    }

    return false;
}

cv::Mat Evaluator::augment(const cv::Mat& targetImage,
        const cv::Mat& replacementImage, const cv::Mat& homography) {
    // We use a fixed boundary value (-1.0f) that is out of the pixel dynamic
    // range [0.0f, 1.0f] to represent pixels to be populated by the original
    // (target) region
    cv::Mat augmentedImage;
    cv::warpPerspective(replacementImage, augmentedImage, homography,
            targetImage.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, 
            cv::Scalar(-1.0f, -1.0f, -1.0f));
    targetImage.copyTo(augmentedImage, augmentedImage == -1.0f);

    return augmentedImage;
}


cv::Mat Evaluator::convertPointsToMat(const std::vector<cv::Point>& points) {
    // N X 3, homogeneous coordinates
    cv::Mat pointsMat = cv::Mat::ones(points.size(), TRANSFORM_MAT_SIDE, CV_32F);
    for (uint32_t iPt = 0; iPt < points.size(); iPt++) {
        const cv::Point& point = points[iPt];
        pointsMat.at<float>(iPt, 0) = (float)point.x;
        pointsMat.at<float>(iPt, 1) = (float)point.y;
    }

    return pointsMat;
}

std::vector<cv::Point> Evaluator::convertMatToPoints(const cv::Mat& pointsMat) {
    std::vector<cv::Point> points;
    for (int32_t iPt = 0; iPt < pointsMat.rows; iPt++) {
        float coordScale = 1.0f/pointsMat.at<float>(iPt, 2);
        int32_t x = (int32_t)std::round(coordScale*pointsMat.at<float>(iPt, 0));
        int32_t y = (int32_t)std::round(coordScale*pointsMat.at<float>(iPt, 1));
        points.emplace_back(x, y);
    }

    return points;
}

bool Evaluator::arePointsInside(const std::vector<cv::Point>& points,
        const cv::Rect& rect) {
    for (const cv::Point& point : points) {
        if (!rect.contains(point)) {
            return false;
        }
    }

    return true;
}

}
}

