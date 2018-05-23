/**
 * This class finds descriptive keypoints in an image.
 */

#pragma once

#include <vector>

#include "opencv2/core.hpp"

#include "core/CircleBuilder.hpp"
#include "core/Definitions.hpp"

namespace core {

class KeypointDetector {
private:
    // Radius of the circle used in the FAST algorithm
    static constexpr int32_t radius = 3;
    // Pixel threshold used in the FAST algorithm
    static constexpr float pixelThresh = 0.15f;

    // Test using points oriented in a diamond, used for the
    // fast rejection filter
    static const std::vector<cv::Point> diamondPoints;
    static constexpr uint32_t diamondThresh = 3u;
    // Test using points oriented in a circle, slower, used
    // only if the fast rejection filter passes
    static const std::vector<cv::Point> circlePoints;
    static constexpr uint32_t circleThresh = 12u;
public:
    /** 
     * Finds salient, descriptive keypoints from the input image.
     *
     * @param image Input image to find salient keypoints from
     * @return The salient keypoints
     */
    std::vector<cv::Point> execute(const cv::Mat& image) const;
private:
    /** 
     * Verifies that the input image has the proper properties required by the algorithm.
     * 
     * @param image The input image to validate
     */
    void validateImage(const cv::Mat& image) const;

    /** 
     * Checks if keypoint candidate is "strong" enough to be considered a corner point
     * according to the passed in surrounding points.
     * 
     * @param image The input image
     * @param keypointCandidate The keypoint location
     * @param surrPoints The surrounding points to compare against
     * @param surrThresh Threshold to control the strictness of the algorithm
     * @return Indicator that the candidate is strong enough to pass the test
     */
    bool isCandidateStrong(const cv::Mat& image, const cv::Point& keypointCandidate,
            const std::vector<cv::Point>& surrPoints, uint32_t surrThresh) const;

    /** 
     * Obtains the differences between the pixel values of the surrounding points
     * and the pixel value of the center point keypointCandidate.
     * 
     * @param image The input image
     * @param keypointCandidate The keypoint location
     * @param surrPoints The surrounding points to obtain values from
     * @return Aforementioned differences
     */
    std::vector<float> getPointDiffs(const cv::Mat& image,
            const cv::Point& keypointCandidate, const std::vector<cv::Point>& surrPoints) const;

    /** 
     * Computes the max number of (circularly) contiguous values that are larger than
     * the threshold.
     * 
     * @param values The list of values
     * @param thresh The threshold to compare each value against
     * @return Max number of contiguous values as described above
     */
    uint32_t computeNumContiguousValues(const std::vector<float>& values,
            float thresh) const;
};

}

