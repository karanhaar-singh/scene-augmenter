#include "core/CircleBuilder.hpp"

#include <algorithm>
#include <iterator>

#include "opencv2/imgproc.hpp"

namespace core {

std::vector<cv::Point> CircleBuilder::execute(uint32_t radius) {
    const std::vector<cv::Point> octant = buildOctant(radius);
    const std::vector<cv::Point> circle = buildCircleFromOctant(octant);
    return circle;
}

/*
 * Implementation: Midpoint circle algorithm
 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
std::vector<cv::Point> CircleBuilder::buildOctant(uint32_t radius) {
    // Current point to draw
    cv::Point currPoint((int32_t)radius, 0);

    std::vector<cv::Point> octantPoints;
    // Draw octant points
    while (currPoint.x >= -currPoint.y) {
        octantPoints.push_back(currPoint);

        const bool decrementX = shouldDecrementX(currPoint, radius);
        currPoint.y--;
        if (decrementX) {
            currPoint.x--;
        }
    }

    return octantPoints;
}

/*
 * Implementation: Midpoint circle algorithm
 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
bool CircleBuilder::shouldDecrementX(const cv::Point& point, uint32_t radius) {
    const int32_t errorDiff =
            2*((point.x*point.x + point.y*point.y - (int32_t)(radius*radius)) +
            (-2*point.y + 1)) + (1 - 2*point.x);

    const bool decrementX = (errorDiff > 0);
    return decrementX;
}

std::vector<cv::Point> CircleBuilder::buildCircleFromOctant(
        const std::vector<cv::Point>& octant) {
    // List of transformative functions to apply to an octant to generate a circle
    const std::vector<std::function<cv::Point(const cv::Point&)>>
            functionsToApply{negAndSwapCoords, negateX, negateY};

    std::vector<cv::Point> allMirroredPoints = octant;
    // Mirror the points on the output of the prior operation
    // octant -> quadrant -> semi-circle -> circle
    for (const auto& functionToApply : functionsToApply) {
        allMirroredPoints = addMirroredPoints(allMirroredPoints, functionToApply);
    }

    // Remove duplicate points in the circle
    std::vector<cv::Point> circle;
    std::unique_copy(allMirroredPoints.cbegin(), allMirroredPoints.cend(),
            std::back_inserter(circle));
    if (circle.size() > 1) {
        circle.pop_back();
    }

    return circle;
}

std::vector<cv::Point> CircleBuilder::addMirroredPoints(
        const std::vector<cv::Point>& points,
        const std::function<cv::Point(const cv::Point&)>& mirrorFunction) {
    // Build mirrored points
    std::vector<cv::Point> mirroredPoints;
    std::transform(points.cbegin(), points.cend(),
            std::back_inserter(mirroredPoints), mirrorFunction);

    // Merge mirrored points with input
    std::vector<cv::Point> outputPoints(points.cbegin(), points.cend());
    outputPoints.insert(outputPoints.end(),
            mirroredPoints.crbegin(), mirroredPoints.crend());

    return outputPoints;
}

/*
 * Mirror functions
 */
cv::Point CircleBuilder::negAndSwapCoords(const cv::Point& point) {
    return cv::Point(-point.y, -point.x);
}
cv::Point CircleBuilder::negateX(const cv::Point& point) {
    return cv::Point(-point.x, point.y);
}
cv::Point CircleBuilder::negateY(const cv::Point& point) {
    return cv::Point(point.x, -point.y);
}

}

