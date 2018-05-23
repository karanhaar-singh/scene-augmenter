/**
 * This class applies a homography transformation to an input.
 */

#pragma once

#include <vector>

#include "opencv2/core.hpp"

namespace core {
namespace homography {

class Evaluator {
public:
    /** 
     * Applies a homography to a list of points.
     *
     * @param inputPts Points to transform using a homography 
     * @param homography The homography transform matrix
     * @return The transformed points
     */
    static std::vector<cv::Point> applyPoints(
            const std::vector<cv::Point>& inputPts, const cv::Mat& homography);

    /** 
     * Does a fast sanity check to see if augmentation makes sense.
     *
     * @param replacementPoints Points to transform
     * @param targetRegion ROI of transformed points
     * @param homography The homography transform matrix
     * @return Indicator that the sanity check passed
     */
    static bool isAugmentationSane(const std::vector<cv::Point>& replacementPoints,
            const cv::Rect& targetRegion, const cv::Mat& homography);

    /** 
     * Applies a homography to a (replacement) image to augment another (target) image.
     *
     * @param targetImage Image to augment
     * @param replacementImage Image to transform onto the target image
     * @param homography The homography transform matrix
     * @return The augmented image
     */
    static cv::Mat augment(const cv::Mat& targetImage,
            const cv::Mat& replacementImage, const cv::Mat& homography);
private:
    /** 
     * Utility routine that converts the representation of a list of points
     * from a vector of cv::Points to matrix form to facilitate easier
     * transformation.
     *
     * @param points Vector representation of points to convert
     * @return Matrix representation of the points
     */
    static cv::Mat convertPointsToMat(const std::vector<cv::Point>& points);

    /** 
     * Utility routine that converts the representation of a list of points
     * from matrix form to a vector of cv::Points.
     *
     * @param pointsMat Matrix representation of points to convert
     * @return Vector representation of points
     */
    static std::vector<cv::Point> convertMatToPoints(const cv::Mat& pointsMat);

    /** 
     * Utility routine that checks if a list of points are inside a given
     * ROI.
     *
     * @param points The list of points to validate
     * @param rect The ROI of interest
     * @return Indicator that every point lies within the ROI
     */
    static bool arePointsInside(const std::vector<cv::Point>& points,
            const cv::Rect& rect);
};

}
}

