#include "shared/ImageConversionUtils.hpp"

#include "opencv2/imgproc.hpp"

#include "shared/Definitions.hpp"

namespace shared {

// We support grayscale and color images, where each can be represented by
// uint8 pixels (for image read/writes to disk) or float32 pixels (for data
// processing).
const std::unordered_set<int32_t> ImageConversionUtils::validConvertMatTypes = {
            CV_8UC1, CV_8UC3, CV_32FC1, CV_32FC3};

cv::Mat ImageConversionUtils::convertToGrayFloats(const cv::Mat& input) {
    validateImage(input, validConvertMatTypes);

    const cv::Mat grayImage = convertToGray(input);
    cv::Mat grayImageFloat;
    grayImage.convertTo(grayImageFloat, CV_32F, INVERSE_MAX_CHAR_VALUE);

    return grayImageFloat;
}

cv::Mat ImageConversionUtils::convertToColorFloats(const cv::Mat& input) {
    validateImage(input, validConvertMatTypes);

    const cv::Mat colorImage = convertToColor(input);
    cv::Mat colorImageFloat;
    colorImage.convertTo(colorImageFloat, CV_32FC3, INVERSE_MAX_CHAR_VALUE);

    return colorImageFloat;
}

cv::Mat ImageConversionUtils::convertToColorUint8(const cv::Mat& input) {
    validateImage(input, validConvertMatTypes);

    const cv::Mat colorImage = convertToColor(input);
    cv::Mat colorImageUint8;
    colorImage.convertTo(colorImageUint8, CV_8UC3, MAX_CHAR_VALUE);

    return colorImageUint8;
}

cv::Mat ImageConversionUtils::convertToGray(const cv::Mat& input) {
    cv::Mat grayImage;
    if (input.channels() == 1) {
        grayImage = input;
    } else {
        cv::cvtColor(input, grayImage, CV_BGR2GRAY);
    }

    return grayImage;
}

cv::Mat ImageConversionUtils::convertToColor(const cv::Mat& input) {
    cv::Mat colorImage;
    if (input.channels() == 1) {
        cv::cvtColor(input, colorImage, CV_GRAY2BGR);
    } else {
        colorImage = input;
    }

    return colorImage;
}

void ImageConversionUtils::validateImage(const cv::Mat& image,
        const std::unordered_set<int32_t>& validMatTypes) {
    shared::VALIDATE_ARGUMENT(!image.empty(),
            "shared::ImageConversionUtils: input image can't be empty");
    shared::VALIDATE_ARGUMENT(validMatTypes.count(image.type()) == 1, 
            "shared::ImageConversionUtils: input image is of wrong type");
}

}

