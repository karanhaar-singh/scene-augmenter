/**
 * This class represents an image/point transformation.
 */

#pragma once

#include <vector>

#include "opencv2/core.hpp"

namespace core {

class Transformation {
private:
    bool isTransformationValid = false;
    cv::Mat transformation;
public:
    /** 
     * Estimates a transformation between the matching points.
     *
     * @param fromPts Source points in the matching pairs
     * @param toPts Target points in the matching pairs
     */
    void build(const std::vector<cv::Point>& fromPts,
            const std::vector<cv::Point>& toPts);

    /** 
     * Checks if the object represents a valid transformation.
     *
     * @return Indicator that this object represents a valid transformation
     */
    bool isValid() const;

    /** 
     * Transforms the inputPoints if possible.
     *
     * @param inputPts The points to transform
     * @return The transformed points or an empty vector if transformation
     *         is not possible
     */
    std::vector<cv::Point> apply(const std::vector<cv::Point>& inputPts) const;

    /** 
     * Transforms the replacementImage onto the targetImage if possible.
     *
     * @param targetImage The image to be augmented onto
     * @param replacementImage The image to transform onto the targetImage
     * @return The augmented image or a copy of the target image if
     *         augmentation is not possible
     */
    cv::Mat augment(const cv::Mat& targetImage,
            const cv::Mat& replacementImage) const;
};

}

