#include "core/FeatureModelGenerator.hpp"

#include <cmath>

#include "shared/Definitions.hpp"

namespace core {

cv::Rect FeatureModelGenerator::getRoi(const cv::Size& _templateSize) {
    // Bound the ROI by the largest square region within the template
    // rectangle
    const int32_t templateXRadius = (_templateSize.width - 1)/2;
    const int32_t templateYRadius = (_templateSize.height - 1)/2;
    const int32_t minTemplateRadius = std::min(
            templateXRadius, templateYRadius);

    // Reduce the ROI radius by a factor of 1/sqrt(2) to ensure all rotations
    // will still remain within the full template size
    const int32_t roiRadius = (int32_t)std::floor(
            ONE_OVER_SQRT_TWO*((float)minTemplateRadius));

    // Build the final ROI
    const int32_t roiSide = 2*roiRadius + 1;
    const cv::Rect roi(templateXRadius - roiRadius, templateYRadius - roiRadius,
            roiSide, roiSide);

    return roi;
}

FeatureModelGenerator::FeatureModelGenerator(const std::string& _modelPath,
        const cv::Size& _templateSize, uint32_t _numAngleBuckets) :
        modelPath(_modelPath), templateSize(_templateSize),
        numAngleBuckets(_numAngleBuckets), randomNumberGenerator(randomDevice()) {
    const cv::Rect roi = getRoi(templateSize);
    xPointGenerator = std::uniform_int_distribution<int32_t>(
            roi.x, roi.x + roi.width - 1);
    yPointGenerator = std::uniform_int_distribution<int32_t>(
            roi.y, roi.y + roi.height - 1);
}

void FeatureModelGenerator::execute() {
    // First generate the base model (orientation = 0 radians)
    const std::vector<cv::Point> patch1Points = selectRandomPoints();
    const std::vector<cv::Point> patch2Points = selectRandomPoints();
    const FeatureModelAtAngle baseModel(patch1Points, patch2Points);

    // Generate multiple rotations at angles uniformly spaced between 0 and 2pi
    // radians, numAngleBuckets controls the quantization granularity
    std::vector<FeatureModelAtAngle> rotatedModels;
    for (uint32_t iBucket = 0; iBucket < numAngleBuckets; iBucket++) {
        const float currAngle = shared::TWO_PI*((float)iBucket/(float)numAngleBuckets);
        const FeatureModelAtAngle rotatedModel = rotateModel(
                baseModel, currAngle, templateSize);
        rotatedModels.push_back(rotatedModel);
    }

    // Write to disk
    FeatureModel featureModel(templateSize, numAngleBuckets, rotatedModels);
    featureModel.write(modelPath);
}

std::vector<cv::Point> FeatureModelGenerator::selectRandomPoints() {
    std::vector<cv::Point> patchPoints;
    for (uint32_t iBit = 0; iBit < NUM_BRIEF_BITS; iBit++) {
        const int32_t x = xPointGenerator(randomNumberGenerator);
        const int32_t y = yPointGenerator(randomNumberGenerator);
        patchPoints.emplace_back(x, y);
    }

    return patchPoints;
}

FeatureModelAtAngle FeatureModelGenerator::rotateModel(
        const FeatureModelAtAngle& _baseModel, float angle,
        const cv::Size& _templateSize) const {
    FeatureModelAtAngle rotatedModel;
    for (uint32_t iPt = 0; iPt < NUM_BRIEF_BITS; iPt++) {
        // Rotate the first point of all point pairs
        const cv::Point& point1 = _baseModel.points1[iPt];
        const cv::Point rotPoint1 = rotatePoint(point1,
                angle, _templateSize);
        rotatedModel.points1.push_back(rotPoint1);

        // Rotate the second point of all point pairs
        const cv::Point& point2 = _baseModel.points2[iPt];
        const cv::Point rotPoint2 = rotatePoint(point2,
                angle, _templateSize);
        rotatedModel.points2.push_back(rotPoint2);
    }

    return rotatedModel;
}

cv::Point FeatureModelGenerator::rotatePoint(const cv::Point& point, float angle,
        const cv::Size& templateSize) const {
    // Convert point to float space, rotate, then convert back to the int space
    const cv::Point2f pointF((float)point.x, (float)point.y);
    const cv::Point2f rotPointF = rotatePoint2f(pointF, angle, templateSize);
    const cv::Point rotPoint((int32_t)std::round(rotPointF.x),
            (int32_t)std::round(rotPointF.y));

    // Ensure that rotated point is in the range of the patch
    const cv::Point clampedRotPoint(
            std::min(std::max(rotPoint.x, 0), templateSize.width - 1),
            std::min(std::max(rotPoint.y, 0), templateSize.height - 1));

    return clampedRotPoint;
}

cv::Point2f FeatureModelGenerator::rotatePoint2f(const cv::Point2f& point, float angle,
        const cv::Size& templateSize) const {
    // Map point from image space to the 2D euclidean space by centering and flipping
    // the y axis of the image 
    const cv::Point2f shift(
            (float)(templateSize.width - 1)/2.0f,
            (float)(templateSize.height - 1)/2.0f);
    const cv::Point2f cenPoint(point.x - shift.x, shift.y - point.y);
    // Apply rotation matrix
    const cv::Point2f rotCenPoint(
            cenPoint.x*std::cos(angle) - cenPoint.y*std::sin(angle),
            cenPoint.x*std::sin(angle) + cenPoint.y*std::cos(angle));
    // Map point back to image space
    const cv::Point2f rotPoint(rotCenPoint.x + shift.x, shift.y - rotCenPoint.y);

    return rotPoint;
}

}

