/**
 * Internal header of SceneAugmenter, see SceneAugmenter.hpp for high level documentation
 */

#pragma once

#include <string>
#include <vector>

#include "opencv2/core.hpp"

#include "core/Definitions.hpp"
#include "core/KeypointDetector.hpp"

#include "CorrespondenceFinder.hpp"
#include "Definitions.hpp"
#include "SceneFeatureExtractor.hpp"
#include "TransformationFitter.hpp"

class SceneAugmenterPri {
private:
    /**
     * Contains useful information of a single image used to perform augmentation
     * against another image (which is also described by an ImageDescription object).
     */
    struct ImageDescription {
        // Dimensions of the image
        cv::Size size;
        // Keypoints of the image, may contain spurious detections
        std::vector<cv::Point> keypoints;
        // The respective feature vectors describing the aforementioned keypoints
        std::vector<core::FeatureVector> featureVectors;

        ImageDescription(const cv::Size& _size,
                const std::vector<cv::Point>& _keypoints,
                const std::vector<core::FeatureVector>& _featureVectors) :
                size(_size), keypoints(_keypoints),
                featureVectors(_featureVectors) {};
        ImageDescription() {};
    };
private:
    /**
     * The core modules that facilitate SceneAugmentation.
     */
    core::KeypointDetector keypointDetector;
    SceneFeatureExtractor sceneFeatureExtractor;
    CorrespondenceFinder correspondenceFinder;
    TransformationFitter transformationFitter;

    /**
     * Persistent data structures that store precomputed information of both
     * the source and replacement images.
     */
    ImageDescription sourceImageDescription;
    cv::Mat replacementImageFloat;
public:
    /** 
     * Internal public interface, see SceneAugmenter.hpp for full documentation
     */
    SceneAugmenterPri(const std::string& modelPath) :
            keypointDetector{}, sceneFeatureExtractor{modelPath},
            correspondenceFinder{}, transformationFitter{} {};

    void setSourceImage(const cv::Mat& newSourceImage);
    void setReplacementImage(const cv::Mat& newReplacementImage);
    cv::Mat execute(const cv::Mat& targetImage) const;
private:
    /** 
     * Verifies that the input image has the proper properties required by the
     * algorithm.
     * 
     * @param image The input image to validate
     */
    void validateImage(const cv::Mat& image) const;

    /** 
     * Builds an ImageDescription object from the input image.  This method encapsulates
     * the portion of the pipeline that operates on each image independently.
     * 
     * @param imageToDescribe The image to describe
     * @return ImageDescription object that describes the input image
     */
    ImageDescription buildImageDescription(const cv::Mat& imageToDescribe) const;

    /** 
     * Augments the persistent replacement image on to the targetImage using a given
     * transformation.  The replacement image will be rescaled to fit the source image
     * dimensions
     * 
     * @param targetImage The image to perform augmentation on
     * @param transformation The transformation which mathematically describes the
     *                       augmentation process via a transformation matrix
     * @return The augmented image
     */
    cv::Mat augment(const cv::Mat& targetImage, const core::Transformation& transformation) const;
};

