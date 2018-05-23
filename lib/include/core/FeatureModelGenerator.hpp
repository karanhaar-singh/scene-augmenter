/**
 * This class generates a model that defines a configuration to perform feature extraction.
 */

#pragma once

#include <random>
#include <vector>

#include "opencv2/core.hpp"

#include "core/Definitions.hpp"
#include "core/FeatureModel.hpp"

namespace core {

class FeatureModelGenerator {
private:
    static constexpr float ONE_OVER_SQRT_TWO = 0.7071f;

    // Path to write the generated model
    std::string modelPath;
    // Expected input image size for the FeatureExtractor
    cv::Size templateSize;
    // Number of rotation angles to generate from 0 to 2pi radians
    uint32_t numAngleBuckets;

    // Persistent data members which handles PRNG
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator;
    std::uniform_int_distribution<int32_t> xPointGenerator;
    std::uniform_int_distribution<int32_t> yPointGenerator;
public:
    /** 
     * Builds a new FeatureModelGenerator.
     * 
     * @param _modelPath File path to write out the FeatureModel
     * @param _templateSize Image template size which defines the
     *        size of the expected image to extract a feature
     *        vector from
     * @param _numAngleBuckets The number of different angles used
     *        to precompute the model rotations to achieve rotation
     *        invariance.  A larger number will produce more accurate
     *        rotational-invariant feature extraction at the cost of
     *        space (on disk and memory)
     */
    FeatureModelGenerator(const std::string& _modelPath,
            const cv::Size& _templateSize, uint32_t _numAngleBuckets);

    /** 
     * Generates a new FeatureModel and writes to disk at modelPath.
     */
    void execute();
private:
    /** 
     * Computes the rectangular region within the template that may be
     * populated by points at an orientation of 0 radians.
     * 
     * @param _templateSize The FeatureModel template size
     * @return The aforementioned rectangular region
     */
    static cv::Rect getRoi(const cv::Size& _templateSize);

    /** 
     * Builds a random vector of points.  The number of points is equal
     * to the feature vector length (in our use case of BRIEF) and each
     * point is uniformly selected from the templateSize's ROI (from
     * getRoi).
     * 
     * @return The vector of randomly generated points
     */
    std::vector<cv::Point> selectRandomPoints();

    /** 
     * Rotates the orientation of a model by a specific angle.
     * 
     * @param _baseModel The original model anchored at 0 radians
     * @param angle The angle to rotate the model in radians
     * @param _templateSize The feature model template size
     * @return The rotated model
     */
    FeatureModelAtAngle rotateModel(
            const FeatureModelAtAngle& _baseModel, float angle,
            const cv::Size& _templateSize) const;

    /** 
     * Rotates the orientation of a point by a specific angle.
     * 
     * @param point The point to rotate
     * @param angle The angle to rotate the point by in radians
     * @param templateSize The feature model template size
     * @return The rotated point
     */
    cv::Point rotatePoint(const cv::Point& point, float angle,
            const cv::Size& templateSize) const;

    /** 
     * Rotates the orientation of a float32 point by a specific angle.
     * 
     * @param point The point to rotate
     * @param angle The angle to rotate the point by in radians
     * @param templateSize The feature model template size
     * @return The rotated point
     */
    cv::Point2f rotatePoint2f(const cv::Point2f& point, float angle,
            const cv::Size& templateSize) const;
};

}

