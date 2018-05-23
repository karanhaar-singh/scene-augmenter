/**
 * This high-level class performs the full scene augmentation pipeline as described in
 * the README.  Scene Augmentation attempts to replace specified planar objects detected
 * in the given scene images with an image of a replacement planar object.
 */

#pragma once

#include <memory>
#include <string>

#include "opencv2/core.hpp"

class SceneAugmenterPri;

class SceneAugmenter {
public:
    /** 
     * Builds a new SceneAugmenter using the given model path.
     *
     * @param modelPath File path to the SceneAugmenter model
     */
    SceneAugmenter(const std::string& modelPath);

    /** 
     * (Re)sets the image of the object to search for and replace in subsequent
     * calls to execute on scene images.  The object must be planar (example:
     * a book cover).
     *
     * @param newSourceImage New image of the object to search for, image data
     *                       must be of type uint8 and can be either a 1-channel
     *                       grayscale image or a 3-channel BGR color image
     */
    void setSourceImage(const cv::Mat& newSourceImage);

    /** 
     * (Re)sets the image of the object to use to replace the source image in
     * subsequent calls to execute on scene images.  The object must be planar
     * (example: a book cover).
     *
     * @param newReplacementImage New image of the object to use as replacement
     *                            to the source object, image data must be of
     *                            type uint8 and can be either a 1-channel grayscale
     *                            image or a 3-channel BGR color image
     */
    void setReplacementImage(const cv::Mat& newReplacementImage);

    /** 
     * Attempts to replace the source object with the replacement object in
     * the target image, if it exists. If the source object is not found or if
     * the algorithm fails, a deep copy of the input image will be returned
     * effectively not augmenting the image.
     *
     * @param targetImage The aforementioned target image, image data
     *                    must be of type uint8 and can be either a 1-channel
     *                    grayscale image or a 3-channel BGR color image
     * @return The augmented target image, or a copy of the target image
     *         if the algorithm fails
     */
    cv::Mat execute(const cv::Mat& targetImage) const;

private:
    std::shared_ptr<SceneAugmenterPri> sceneAugmenterPri;
};

