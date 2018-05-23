/**
 * This class attempts to find a good Transformation between two images based
 * on matching keypoints in the presence of outlier matches.
 */

#pragma once

#include <vector>

#include "opencv2/core.hpp"

#include "core/Definitions.hpp"
#include "core/Transformation.hpp"

#include "MatchingPoints.hpp"

class TransformationFitter {
private:
    // Params that control the behavior of Transformation generation,
    uint32_t maxIters = 50000u;
    uint32_t minNonTrivialInliers = 2u;
    float epsilon = 3.0f;
public:
    /** 
     * Will attempt to fit a core::Transformation to the passed in matching points
     * in the presence of outliers.  If the algorithm succeeds, the best fit
     * is returned, otherwise an invalid Transformation object is returned.
     * 
     * @param matches The set of matching points to perform the fit on, which
     *                may contain outliers
     * @return The best fit or an invalid Transformation object if the algorithm
     *         fails
     */
    core::Transformation execute(const MatchingPoints& matches) const;
private:
    /** 
     * Selects a valid subset of matching points, the minimum number required to 
     * build a valid core::Transformation object.
     * 
     * @param matches The set of matching points select a subset from
     * @return The aforementioned subset
     */
    MatchingPoints getMinSubsetMatches(const MatchingPoints& matches) const;

    /** 
     * Computes the number of point pairs that are similar (close enough).
     * 
     * @param points1 First list of points
     * @param points2 Second list of points, paired respectively with the
     *                first list of points
     * @return The number of pairs that have similar points
     */
    uint32_t computeNumSimilarPoints(
                const std::vector<cv::Point>& points1,
                const std::vector<cv::Point>& points2) const;
};

