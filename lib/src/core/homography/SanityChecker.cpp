#include "core/homography/SanityChecker.hpp"

#include <cmath>

#include "opencv2/imgproc.hpp"

#include "shared/Definitions.hpp"

#include "core/homography/Definitions.hpp"

namespace core {
namespace homography {

bool SanityChecker::areMatchesSane(
        const std::vector<cv::Point>& fromPts,
        const std::vector<cv::Point>& toPts) {
    // First check if both shape configurations are degenerate
    if (isConfigDegenerate(fromPts) || isConfigDegenerate(toPts)) {
        return false;
    }

    // Next, verify that both shapes have a consistent orientation, which
    // ensures that planar transformation between the two shapes is sensible
    const std::vector<uint32_t> fromOrientation = getOrientationDescriptor(fromPts);
    const std::vector<uint32_t> toOrientation = getOrientationDescriptor(toPts);
    const bool orientationsAlign = (fromOrientation == toOrientation);

    return orientationsAlign;
}

bool SanityChecker::isConfigDegenerate(const std::vector<cv::Point>& points) {
    assert(points.size() == MIN_BUILD_POINTS);

    // Checks if any three points are collinear
    for (uint32_t iToDel = 0; iToDel < MIN_BUILD_POINTS; iToDel++) {
        std::vector<cv::Point> selPoints = points;
        selPoints.erase(selPoints.begin() + iToDel);
        const bool pointsAreCollinear = arePointsCollinear(selPoints);
        if (pointsAreCollinear) {
            return true;
        }
    }

    return false;
}

bool SanityChecker::arePointsCollinear(const std::vector<cv::Point>& points) {
    assert(points.size() == MIN_BUILD_POINTS - 1);

    // Note: Potential for integer overflow
    const int32_t signedArea =
            (points[0].x*points[1].y + points[1].x*points[2].y + points[2].x*points[0].y) -
            (points[0].x*points[2].y + points[1].x*points[0].y + points[2].x*points[1].y);
    return (signedArea == 0);
}

std::vector<uint32_t> SanityChecker::getOrientationDescriptor(
        const std::vector<cv::Point>& points) {
    // Get mean point
    cv::Point2f meanPoint;
    for (const cv::Point& point : points) {
        meanPoint += static_cast<cv::Point2f>(point);
    }
    meanPoint /= (float)points.size();

    // Compute angles
    std::vector<float> angles;
    for (const cv::Point& point : points) {
        const cv::Point2f centerPoint = static_cast<cv::Point2f>(point) - meanPoint;
        const float angle = std::atan2(centerPoint.y, centerPoint.x);
        angles.push_back(angle);
    }

    // Compute rotation-invariant orientation descriptor
    const std::vector<uint32_t> indices = sortIndices(angles);
    const auto anchorLoc = std::find(indices.cbegin(), indices.cend(), 0);
    std::vector<uint32_t> orientationDescriptor;
    std::rotate_copy(indices.cbegin(), anchorLoc, indices.cend(),
            std::back_inserter(orientationDescriptor));

    return orientationDescriptor;
}

std::vector<uint32_t> SanityChecker::sortIndices(
        const std::vector<float>& input) {
    std::vector<uint32_t> indices(input.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(),
       [&input](uint32_t i1, uint32_t i2) {return input[i1] < input[i2];});

    return indices;
}

}
}

