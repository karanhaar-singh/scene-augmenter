/**
 * This class builds a sequence of points that are oriented in a circle of a given
 * radius.
 */

#pragma once

#include <functional>
#include <vector>

#include "opencv2/core.hpp"

#include "core/Definitions.hpp"

namespace core {

class CircleBuilder {
public:
    /** 
     * Builds a sequence of points that are oriented in a circle centered
     * around the origin.
     * 
     * @param radius The radius of the circle to generate
     * @return The list of points that form the circle, ordered by angle
     *         with respect to the origin
     */
    static std::vector<cv::Point> execute(uint32_t radius);
private:
    /** 
     * Builds a sequence of points that are oriented in the first octant (1/8 of
     * a circle) centered around the origin.
     * 
     * @param radius The radius of the octant to generate
     * @return The list of points that form the octant, ordered by angle
     *         with respect to the origin
     */
    static std::vector<cv::Point> buildOctant(uint32_t radius);

    /** 
     * Helper method used in the midpoint circle algorithm to generate
     * the octant.  Indicates if the next point of the octant to draw
     * should have its x coordinate decremented.
     * 
     * @param point Most recently drawn point
     * @param radius The radius of the octant to generate
     * @return Indicator that the x coordinate of the next
     *         drawn point should be decremented
     */
    static bool shouldDecrementX(const cv::Point& point, uint32_t radius);

    /** 
     * Given the first octant of a circle, generates the entire circle.
     * 
     * @param octant The list of points representing the first octant
     * @return A list of points representing the entire circle
     */
    static std::vector<cv::Point> buildCircleFromOctant(
            const std::vector<cv::Point>& octant);

    /** 
     * Mirror the points based on a passed in mirroring function and add these
     * mirrored points to the original list of points.
     * 
     * @param points Points to mirror
     * @param mirrorFunction Function used to mirror a single point
     * @return A list of points representing the original points merged with 
     *         the mirrored points, is double the size of the input
     */
    static std::vector<cv::Point> addMirroredPoints(
            const std::vector<cv::Point>& points,
            const std::function<cv::Point(const cv::Point&)>& mirrorFunction);

    /** 
     * Negates and swaps the coordinates of the input point.
     *
     * @param point Point to transform
     * @return The transformed point
     */
    static cv::Point negAndSwapCoords(const cv::Point& point);
    /** 
     * Negates the x coordinate of the input point.
     *
     * @param point Point to transform
     * @return The transformed point
     */
    static cv::Point negateX(const cv::Point& point);
    /** 
     * Negates the y coordinate of the input point.
     *
     * @param point Point to transform
     * @return The transformed point
     */
    static cv::Point negateY(const cv::Point& point);
};

}

