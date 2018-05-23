#include "core/KeypointDetector.hpp"

#include <algorithm>

#include "opencv2/imgproc.hpp"

#include "shared/Definitions.hpp"

namespace core {

using KD = KeypointDetector;

const std::vector<cv::Point> KeypointDetector::diamondPoints{
        {KD::radius, 0}, {0, -KD::radius},
        {-KD::radius, 0}, {0, KD::radius}};
const std::vector<cv::Point> KeypointDetector::circlePoints =
        core::CircleBuilder::execute((uint32_t)KD::radius);

/**
 * Algorithm: FAST - naive implementation
 * Rosten, Edward; Drummond, Tom (2006).
 * "Machine Learning for High-speed Corner Detection"
 */
std::vector<cv::Point> KeypointDetector::execute(const cv::Mat& image) const {
    validateImage(image);

    std::vector<cv::Point> cornerPoints;
    // Check each row for keypoints in parallel
    #pragma omp parallel for schedule(static)
    for (int32_t iRow = radius; iRow < image.rows - radius; iRow++) {
        for (int32_t iCol = radius; iCol < image.cols - radius; iCol++) {
            const cv::Point currPoint(iCol, iRow);

            // Fast rejection filter, ensure diamond descriptor is strong to continue
            const bool diamondIsStrong = isCandidateStrong(
                    image, currPoint, diamondPoints, diamondThresh);
            if (diamondIsStrong) {
                // Slower second stage test, ensure full circle descriptor is strong
                const bool circleIsStrong = isCandidateStrong(
                        image, currPoint, circlePoints, circleThresh);
                if (circleIsStrong) {
                    #pragma omp critical
                    {
                        cornerPoints.push_back(currPoint);
                    }
                }
            }
        }
    }

    return cornerPoints;
}

void KeypointDetector::validateImage(const cv::Mat& image) const {
    shared::VALIDATE_ARGUMENT(!image.empty(),
            "core::KeypointDetector: input image can't be empty");
    shared::VALIDATE_ARGUMENT(image.type() == CV_32FC1, 
            "core::KeypointDetector: input image is of wrong type");
}

/**
 * Algorithm: FAST - naive implementation
 * Rosten, Edward; Drummond, Tom (2006).
 * "Machine Learning for High-speed Corner Detection"
 */
bool KeypointDetector::isCandidateStrong(const cv::Mat& image,
        const cv::Point& keypointCandidate, const std::vector<cv::Point>& surrPoints, 
        uint32_t surrThresh) const {

    const std::vector<float> diffs = getPointDiffs(image, keypointCandidate, surrPoints); 

    // Check if candidate keypoint is strong in one direction
    const uint32_t numContiguousPosVals = computeNumContiguousValues(diffs, pixelThresh);
    if (numContiguousPosVals >= surrThresh) {
        return true;
    }

    // Check if candidate keypoint is strong in the other direction
    std::vector<float> negDiffs;
    std::transform(diffs.cbegin(), diffs.cend(), std::back_inserter(negDiffs), std::negate<float>());
    const uint32_t numContiguousNegVals = computeNumContiguousValues(negDiffs, pixelThresh);
    if (numContiguousNegVals >= surrThresh) {
        return true;
    }

    return false;
}

std::vector<float> KeypointDetector::getPointDiffs(const cv::Mat& image,
        const cv::Point& keypointCandidate, const std::vector<cv::Point>& surrPoints) const {
    // Save pixel value at keypoint candidate
    const float candidateValue = image.at<float>(keypointCandidate);

    // Get differences between the surrounding points and the center point pixel values
    std::vector<float> values;
    for (const cv::Point& surrPoint : surrPoints) {
        const float value = image.at<float>(surrPoint + keypointCandidate) - candidateValue;
        values.push_back(value);
    }

    return values;
}

uint32_t KeypointDetector::computeNumContiguousValues(const std::vector<float>& values,
        float pixelThresh) const {
    // If all values are above the pixel threshold, the full vector of values are contiguous
    const auto minLoc = std::min_element(values.cbegin(), values.cend());
    if (*minLoc >= pixelThresh) {
        return values.size();
    }

    // Otherwise, anchor at one of the breaks
    const uint32_t startIndex = std::distance(values.cbegin(), minLoc);

    // Now, compute the maximum number of contiguous values
    uint32_t maxNumContigPoints = 0u;
    uint32_t currNumContigPoints = 0u;
    for (uint32_t iValStart = 0; iValStart < values.size(); iValStart++) {
        const uint32_t iVal = (iValStart + startIndex) % values.size();
        if (values[iVal] > pixelThresh) {
            currNumContigPoints++;
            if (maxNumContigPoints < currNumContigPoints) {
                maxNumContigPoints = currNumContigPoints;
            }
        } else {
            currNumContigPoints = 0u;
        }
    }

    return maxNumContigPoints;
}

}

