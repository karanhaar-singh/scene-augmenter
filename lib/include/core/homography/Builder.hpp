/**
 * This class builds a homography from matching points.
 */

#pragma once

#include <string>
#include <vector>

#include "opencv2/core.hpp"

#include "shared/Definitions.hpp"

#include "core/homography/Definitions.hpp"

namespace core {
namespace homography {

class Builder {
private:
    static constexpr uint32_t numRowsA = 2*MIN_BUILD_POINTS;
    static constexpr uint32_t numColsA = numRowsA + 1u;
public:
    /** 
     * Builds a homography that defines a transformation between the passed in
     * matching points.
     *
     * @param fromPts Source points in the matching pairs
     * @param toPts Target points in the matching pairs
     * @return The homography matrix
     */
    static cv::Mat build(const std::vector<cv::Point>& fromPts,
            const std::vector<cv::Point>& toPts);
private:
    /** 
     * Builds the "A" matrix, which represents the system of 8 equations used to
     * build a homography.
     *
     * @param fromPts Source points in the matching pairs
     * @param toPts Target points in the matching pairs
     * @return The "A" matrix
     */
    static cv::Mat buildA(const std::vector<cv::Point>& fromPts,
            const std::vector<cv::Point>& toPts);
};

}
}

