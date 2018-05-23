/**
 * This class extracts feature vectors to describe specific keypoint locations in images.
 * Such feature vectors make it much easier to find correspondences between two images.
 */

#pragma once

#include <string>
#include <vector>

#include "opencv2/core.hpp"

#include "core/Definitions.hpp"
#include "core/FeatureExtractor.hpp"

#include "Definitions.hpp"

class SceneFeatureExtractor {
private:
    // The core feature extractor object, describes an image crop by a feature vector
    core::FeatureExtractor featureExtractor;
public:
    /** 
     * Builds a new SceneFeatureExtractor that will extract features using configuration
     * information from the given model.
     *
     * @param modelPath File path to the SceneFeatureExtractor model
     */
    SceneFeatureExtractor(const std::string& modelPath) : featureExtractor(modelPath) {};

    /** 
     * Extracts feature vectors at the given keypoint locations from the image.
     *
     * @param image The image to extract feature vectors from
     * @param keypoints The list of points that defines the locations in the image
     *                  to extract feature vectors from
     * @return A list of feature vectors that correspond to each point from the
     *          list of keypoints
     */
    std::vector<core::FeatureVector> execute(const cv::Mat& image,
            const std::vector<cv::Point>& keypoints) const;
private:
    /** 
     * Verifies that the input image has the proper properties required by the
     * algorithm.
     * 
     * @param image The input image to validate
     */
    void validateImage(const cv::Mat& image) const;

    /** 
     * Verifies that the input keypoints has the proper properties required by the
     * algorithm and is jointly internally consistent with the input image.
     * 
     * @param keypoints The list of keypoints to validate
     * @param image The input image to validate
     */
    void validateKeypoints(const std::vector<cv::Point>& keypoints,
            const cv::Mat& image) const;

    /** 
     * Crops a subregion from an image in a way that can handle partially out of
     * bounds crop regions.
     * 
     * @param image The input image to crop
     * @param imageRoi The region to crop from the input image, may be partially out
     *                 of bounds of the image region
     * @return The cropped subregion
     */
    cv::Mat doSafeCrop(const cv::Mat& image, const cv::Rect& imageRoi) const;
};

