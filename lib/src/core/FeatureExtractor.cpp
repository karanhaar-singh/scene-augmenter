#include "core/FeatureExtractor.hpp"

#include <cmath>

#include "shared/Definitions.hpp"

namespace core {

/**
 * Algorithm: Steered BRIEF
 * Rublee, Ethan, et al. "ORB: An efficient alternative to SIFT or SURF."
 * Computer Vision (ICCV), 2011 IEEE international conference on. IEEE, 2011.
 */
FeatureVector FeatureExtractor::execute(const cv::Mat& inputImage) const {
    validateImage(inputImage);

    const float angle = computeAngle(inputImage);
    const FeatureModelAtAngle featureModelAtAngle =
            selectFeatureModelAtAngle(featureModel, angle);
    const std::bitset<NUM_BRIEF_BITS> featureVector =
            buildFeatureVector(inputImage, featureModelAtAngle);

    return featureVector;
}

cv::Size FeatureExtractor::getTemplateSize() const {
    return featureModel.templateSize;
}

void FeatureExtractor::validateImage(const cv::Mat& image) const {
    shared::VALIDATE_ARGUMENT(!image.empty(),
            "core::FeatureExtractor: input image can't be empty");
    shared::VALIDATE_ARGUMENT(image.size() == getTemplateSize(),
            "core::FeatureExtractor: input image is of wrong size");
    shared::VALIDATE_ARGUMENT(image.type() == CV_32FC1, 
            "core::FeatureExtractor: input image is of wrong type");
}

/**
 * Algorithm: Orientation by Intensity Centroid
 * Rublee, Ethan, et al. "ORB: An efficient alternative to SIFT or SURF."
 * Computer Vision (ICCV), 2011 IEEE international conference on. IEEE, 2011.
 */
float FeatureExtractor::computeAngle(const cv::Mat& image) const {
    const float xCen = (float)(image.cols - 1)/2.0f;
    const float yCen = (float)(image.rows - 1)/2.0f;

    float xMoment = 0.0f;
    float yMoment = 0.0f;
    for (int32_t iRow = 0; iRow < image.rows; iRow++) {
        const float yDist = yCen - (float)iRow; 
        const float yNorm = (yDist*yDist)/(float)(yCen*yCen);
        for (int32_t iCol = 0; iCol < image.cols; iCol++) {
            const float xDist = (float)iCol - xCen; 
            const float xNorm = (xDist*xDist)/(float)(xCen*xCen);
            // Check if we are in the centered elliptical region
            if (yNorm + xNorm <= 1.0f) {
                const float val = image.at<float>(iRow, iCol);
                xMoment += (xDist*val);
                yMoment += (yDist*val);
            }
        }
    }

    const float angle = std::atan2(yMoment, xMoment);
    return angle;
}

FeatureModelAtAngle FeatureExtractor::selectFeatureModelAtAngle(
        const FeatureModel& featureModel, float angle) const {
    // Quantize the angle to an uint32,
    float bucketChoice = (angle*(float)featureModel.numAngleBuckets)/shared::TWO_PI;
    if (bucketChoice < 0.0f) {
        bucketChoice += (float)featureModel.numAngleBuckets;
    }
    bucketChoice = std::min(std::max(bucketChoice, 0.0f),
            (float)featureModel.numAngleBuckets);
    const uint32_t bucketIndex = (uint32_t)std::round(bucketChoice) %
            featureModel.numAngleBuckets;

    // Use the quantized angle as an index to select the proper angle-specific
    // submodel
    const FeatureModelAtAngle selectedModel =
            featureModel.featureModelAtAngles[bucketIndex];

    return selectedModel;
}

/**
 * Algorithm: BRIEF
 * Calonder, Michael, et al. "BRIEF: Computing a local binary descriptor very fast."
 * IEEE Transactions on Pattern Analysis and Machine Intelligence 34.7 (2012): 1281-1298.
 */
std::bitset<NUM_BRIEF_BITS> FeatureExtractor::buildFeatureVector(
        const cv::Mat& image, const FeatureModelAtAngle& featureModelAtAngle) const {
    std::bitset<NUM_BRIEF_BITS> briefVector;
    for (uint32_t iBit = 0; iBit < NUM_BRIEF_BITS; iBit++) {
        const cv::Point& point1 = featureModelAtAngle.points1[iBit];
        const cv::Point& point2 = featureModelAtAngle.points2[iBit];
        if (image.at<float>(point1) > image.at<float>(point2)) {
            briefVector.set(iBit);
        }
    }

    return briefVector;
}

}

