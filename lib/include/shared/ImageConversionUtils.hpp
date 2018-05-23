/**
 * Utility class that provides useful routines for converting images between
 * different formats.
 */

#pragma once

#include <unordered_set>
#include <vector>

#include "opencv2/core.hpp"

#include "shared/Definitions.hpp"

namespace shared {

class ImageConversionUtils {
private:
    // Valid cv::Mat types that the class can handle
    static const std::unordered_set<int32_t> validConvertMatTypes;
public:
    /** 
     * Converts the input image to a 1-channel cv::Mat with float32 pixels
     * in the range of [0.0f, 1.0f] to represent a grayscale image.
     * 
     * @param input The image to convert
     * @return The converted image
     */
    static cv::Mat convertToGrayFloats(const cv::Mat& input);

    /** 
     * Converts the input image to a 3-channel BGR cv::Mat with float32 pixels
     * in the range of [0.0f, 1.0f] to represent a color image.
     * 
     * @param input The image to convert
     * @return The converted image
     */
    static cv::Mat convertToColorFloats(const cv::Mat& input);

    /** 
     * Converts the input image to a 3-channel BGR cv::Mat with uint8 pixels
     * in the range of [0, 255] to represent a color image.
     * 
     * @param input The image to convert
     * @return The converted image
     */
    static cv::Mat convertToColorUint8(const cv::Mat& input);
private:
    /** 
     * Converts the input image to a 1-channel grayscale cv::Mat.
     * 
     * @param input The image to convert, must be either a 1-channel
     *              grayscale or a 3-channel BGR color image
     * @return The converted image
     */
    static cv::Mat convertToGray(const cv::Mat& input);

    /** 
     * Converts the input image to a 3-channel BGR color cv::Mat.
     * 
     * @param input The image to convert, must be either a 1-channel
     *              grayscale or a 3-channel BGR color image
     * @return The converted image
     */
    static cv::Mat convertToColor(const cv::Mat& input);

    /** 
     * Validates the input image using a set of reference valid cv::Mat
     * types.
     * 
     * @param image The image to validate 
     * @param validMatTypes The valid cv:Mat types
     */
    static void validateImage(const cv::Mat& image,
            const std::unordered_set<int32_t>& validMatTypes);
};

}

