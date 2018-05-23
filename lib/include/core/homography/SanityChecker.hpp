/**
 * This class performs some fast validation checks on matching points to determine
 * if proceeding with building a homography is sensible.
 */

#pragma once

#include <vector>

#include "opencv2/core.hpp"

namespace core {
namespace homography {

class SanityChecker {
public:
    /** 
     * Check the matching points to determine if proceeding with building
     * a homography is sensible.
     *
     * @param fromPts sOurce points in the matching pairs
     * @param toPts Target points in the matching pairs
     * @return Indicator that these matches can be used to build a homography
     */
    static bool areMatchesSane(const std::vector<cv::Point>& fromPts,
            const std::vector<cv::Point>& toPts);
private:
    /** 
     * Checks if the list of points are in a degenerate configuration.
     *
     * @param points List of points to validate 
     * @return Indicator that these points are in a degenerate configuration
     */
    static bool isConfigDegenerate(const std::vector<cv::Point>& points);

    /** 
     * Checks if the list of points are collinear (lie on the same line).
     *
     * @param points List of points to check for collinearity
     * @return Indicator that these points are collinear
     */
    static bool arePointsCollinear(const std::vector<cv::Point>& points);

    /** 
     * Computes a descriptor that describes the orientation of the shape
     * described by the points.
     *
     * @param points Shape to describe
     * @return The shape descriptor
     */
    static std::vector<uint32_t> getOrientationDescriptor(
            const std::vector<cv::Point>& points);

    /** 
     * Computes a list of indices that correspond to the input list in sorted order.
     *
     * @param points List of values to get sorted indices for
     * @return The sorted indices
     */
    static std::vector<uint32_t> sortIndices(const std::vector<float>& input);
};

}
}

