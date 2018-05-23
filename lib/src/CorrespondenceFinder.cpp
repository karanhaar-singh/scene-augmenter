#include "CorrespondenceFinder.hpp"

#include <limits>

#include "core/FeatureMatcher.hpp"

/**
 * Algorithm: Simple pairwise comparison algorithm.
 */
Correspondences CorrespondenceFinder::execute(
        const std::vector<core::FeatureVector>& sourceFeatureVectors,
        const std::vector<core::FeatureVector>& targetFeatureVectors) const {
    Correspondences correspondences;
    // No matches are possible if either input vector is empty.  Additionally,
    // we must enforce that target list has at least 2 elements so the algorithm
    // used to suppress non-discriminative matches is sensible.
    if (sourceFeatureVectors.size() == 0 || targetFeatureVectors.size() <= 1) {
        return correspondences;
    }

    // Run pairwise comparison algorithm in parallel by parallelizing over source
    // feature vectors.
    #pragma omp parallel for schedule(static)
    for (uint32_t iFeat1 = 0; iFeat1 < sourceFeatureVectors.size(); iFeat1++) {
        const core::FeatureVector& featureVector1 = sourceFeatureVectors[iFeat1];

        // Store the top two matches
        uint32_t bestMatchDist = std::numeric_limits<uint32_t>::max();
        uint32_t nextBestMatchDist = std::numeric_limits<uint32_t>::max();
        uint32_t bestMatchIndex = 0;
        for (uint32_t iFeat2 = 0; iFeat2 < targetFeatureVectors.size(); iFeat2++) {
            const core::FeatureVector& featureVector2 = targetFeatureVectors[iFeat2];
            const uint32_t distance = core::FeatureMatcher::execute(
                    featureVector1, featureVector2);
            if (distance <= bestMatchDist) {
                nextBestMatchDist = bestMatchDist;
                bestMatchDist = distance;
                bestMatchIndex = iFeat2;
            } else if (distance <= nextBestMatchDist) {
                nextBestMatchDist = distance;
            }
        }

        // Non-discriminative match suppression
        if (nextBestMatchDist - bestMatchDist >= minTopDistance) {
            #pragma omp critical
            {
                correspondences.emplace_back(iFeat1, bestMatchIndex);
            }
        }
    }

    sortCorrespondences(correspondences);

    return correspondences;
}

void CorrespondenceFinder::sortCorrespondences(Correspondences& correspondences) const {
    std::sort(correspondences.begin(), correspondences.end(),
            [](const std::pair<uint32_t, uint32_t>& lhs,
                    const std::pair<uint32_t, uint32_t>& rhs) {
                return lhs.first < rhs.first;
            });
}

