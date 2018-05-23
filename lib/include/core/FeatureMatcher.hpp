/**
 * This class computes a match score between two feature vectors.
 */

#pragma once

#include "core/Definitions.hpp"

namespace core {

class FeatureMatcher {
public:
    /** 
     * Computes a score that gives a measure of how "close together" the
     * two passed in feature vectors are.
     *
     * @note OPTIMIZATION: Making this function inline empirically improves performance
     *
     * @param featureVector1 First feature vector 
     * @param featureVector2 Second feature vector
     * @return A measurement of distance between the two feature vectors.
     *         A smaller number indicates that the two feature vectors are
     *         more similar.
     */
    static inline uint32_t execute(const FeatureVector& featureVector1,
            const FeatureVector& featureVector2) {
        const uint32_t hammingDistance = (featureVector1 ^ featureVector2).count();
        return hammingDistance;
    };
};
    
}

