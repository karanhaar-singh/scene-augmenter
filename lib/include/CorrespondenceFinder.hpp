/**
 * This class finds correspondences between two images by matching feature vectors
 * that describe the keypoints within both images.  Ideally, each correspondence should
 * be matching the same salient object found in both images (example: corner of a particular
 * window found in both images).
 */

#pragma once

#include "core/Definitions.hpp"

#include "Definitions.hpp"

class CorrespondenceFinder {
private:
    /**
     * When searching for the best matching target feature vector for a given source
     * feature vector, we want to make sure that the second best matching target
     * feature vector's match score is "far enough" from the best match score which
     * helps reduce non-discriminative matches according to literature.  This threshold
     * controls the required match distance.
     */
    uint32_t minTopDistance = 4u;
public:
    CorrespondenceFinder(uint32_t _minTopDistance) : minTopDistance(_minTopDistance) {};
    CorrespondenceFinder() {};

    /** 
     * Finds correspondences between the source list of feature vectors and the 
     * target list of feature vectors. These correspondences find the "best matching"
     * target feature vector for a given source feature vector. Up to one correspondence
     * is generated for each feature vector from source list.
     * 
     * @param sourceFeatureVectors The source list of feature vectors
     * @param targetFeatureVectors The target list of feature vectors
     * @return The correspondences between the source and the target list of feature
     *         vectors
     */
    Correspondences execute(const std::vector<core::FeatureVector>& sourceFeatureVectors,
            const std::vector<core::FeatureVector>& targetFeatureVectors) const;
private:
    /**
     * Sorts correspondences by the first (source) correspondence index in-place.
     * 
     * @param correspondences The correspondences to sort in-place
     */
    void sortCorrespondences(Correspondences& correspondences) const;
};

