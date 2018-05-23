#include "TransformationFitter.hpp"

#include <cmath>

#include "core/homography/Definitions.hpp"

/**
 * Algorithm: RANSAC
 * https://en.wikipedia.org/wiki/Random_sample_consensus
 */
core::Transformation TransformationFitter::execute(const MatchingPoints& matches) const {
    // No point continuing if there are not enough matches to proceed
    if (matches.fromPts.size() <= core::homography::MIN_BUILD_POINTS) {
        return core::Transformation{};
    }

    core::Transformation bestTransformation;
    uint32_t bestNumInliers = core::homography::MIN_BUILD_POINTS + minNonTrivialInliers - 1;
    
    // Find the best transformation between the matches by generating transformations
    // for different subsets of the matches and selecting the best one based on how
    // each transformation performs on the full set of matches.  We parallelize over
    // individual RANSAC trials.
    #pragma omp parallel for schedule(dynamic)
    for (uint32_t iter = 0; iter < maxIters; iter++) {
        const MatchingPoints minSubsetMatches = getMinSubsetMatches(matches);

        core::Transformation currTransformation;
        currTransformation.build(minSubsetMatches.fromPts, minSubsetMatches.toPts);

        const std::vector<cv::Point> mappedFromPts =
                currTransformation.apply(matches.fromPts); 

        // Transformation building failed if core::Transformation.apply returns no points.
        if (mappedFromPts.size() == 0) {
            continue;
        }

        // Compute number of inliers from all matches, our metric for selecting the
        // best transformation.
        const uint32_t numInliers = computeNumSimilarPoints(mappedFromPts, matches.toPts);
        #pragma omp critical
        {
            if (numInliers > bestNumInliers) {
                bestTransformation = currTransformation;
                bestNumInliers = numInliers;
            }
        }
    }

    return bestTransformation;
}

MatchingPoints TransformationFitter::getMinSubsetMatches(
        const MatchingPoints& matches) const {
    MatchingPoints minSubsetMatches;
    for (uint32_t iIndex = 0; iIndex < core::homography::MIN_BUILD_POINTS; iIndex++) {
        // Select random indices to build the subset, very simple algorithm
        const uint32_t randomIndex = rand() % matches.fromPts.size();
        minSubsetMatches.fromPts.push_back(matches.fromPts[randomIndex]);
        minSubsetMatches.toPts.push_back(matches.toPts[randomIndex]); 
    }

    return minSubsetMatches;
}

uint32_t TransformationFitter::computeNumSimilarPoints(
            const std::vector<cv::Point>& points1,
            const std::vector<cv::Point>& points2) const {
    assert(points1.size() == points2.size());
    const uint32_t numPoints = points1.size();

    uint32_t numSimilarPoints = 0u;
    for (uint32_t iPt = 0; iPt < numPoints; iPt++) {
        // Use L2 distance as our metric
        const float distance = cv::norm(points1[iPt] - points2[iPt]);
        if (distance < epsilon) {
            numSimilarPoints++;
        }
    }

    return numSimilarPoints;
}

