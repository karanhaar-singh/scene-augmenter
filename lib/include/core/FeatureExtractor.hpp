/**
 * This class generates a feature vector to describe the input image.
 */

#pragma once

#include <bitset>
#include <string>

#include "opencv2/core.hpp"

#include "core/Definitions.hpp"
#include "core/FeatureModel.hpp"

namespace core {

class FeatureExtractor {
private:
    static constexpr float epsilon = 1e-30f;
    // Contains configuration information of the FeatureExtractor at all angles
    FeatureModel featureModel;
public:
    /** 
     * Builds a new FeatureExtractor that will extract features using configuration
     * information from the given model path.
     *
     * @param modelPath File path to the FeatureExtractor model
     */
    FeatureExtractor(const std::string& modelPath) : featureModel(modelPath) {};

    /** 
     * Extracts a feature vector that describes the input image.
     *
     * @param inputImage Input image used to extract the feature vector,
     *        image data must be of type float32, a 1-channel grayscale
     *        image, and have the same dimensions as the template size
     *        (given by getTemplateSize())
     * @return The feature vector describing the inputImage
     */
    FeatureVector execute(const cv::Mat& inputImage) const;

    /** 
     * Returns the dimensions of the image expected by execute. 
     *
     * @param The aforementioned expected dimensions
     */
    cv::Size getTemplateSize() const;
private:
    /** 
     * Verifies that the input image has the proper properties required by the algorithm.
     * 
     * @param image The input image to validate
     */
    void validateImage(const cv::Mat& image) const;

    /** 
     * Computes the approximate orientation of the image in radians.
     *
     * @param The image to compute the orientation of
     * @return The angle in radians
     */
    float computeAngle(const cv::Mat& image) const;

    /** 
     * Pull out a FeatureModelAtAngle closest to the specified angle from the specified
     * FeatureModel.
     *
     * @param FeatureModel The full FeatureModel to pull a FeatureModelAtAngle from
     * @param angle The desired orientation of the FeatureModelAtAngle
     * @return The FeatureModelAtAngle
     */
    FeatureModelAtAngle selectFeatureModelAtAngle(
            const FeatureModel& featureModel, float angle) const;

    /** 
     * Extracts a feature vector from the image using the configuration information
     * defined in the FeatureModelAtAngle.
     *
     * @param image The image to extract the feature vector from
     * @param featureModelAtAngle Configuration information to use when extracting features
     * @return The feature vector
     */
    std::bitset<NUM_BRIEF_BITS> buildFeatureVector(
            const cv::Mat& image, const FeatureModelAtAngle& featureModelAtAngle) const;
};

}

