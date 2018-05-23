#include "SceneFeatureExtractor.hpp"

#include "shared/Definitions.hpp"

/**
 * Algorithm: Extract feature vectors from subimage regions centered at each keypoint
 * independently.
 */
std::vector<core::FeatureVector> SceneFeatureExtractor::execute(const cv::Mat& image,
        const std::vector<cv::Point>& keypoints) const {
    // Validate internal consistency of the arguments
    validateImage(image);
    validateKeypoints(keypoints, image);

    const uint32_t numKeypoints = keypoints.size();
    const cv::Size templateSize = featureExtractor.getTemplateSize();

    // Pre-allocate output to facilitate easy parallelization
    std::vector<core::FeatureVector> featureVectors(numKeypoints);

    // For each keypoint, crop an ROI of size templateSize centered at the keypoint,
    // extract and store the feature vector.  We parallelize over the keypoints.
    #pragma omp parallel for schedule(static)
    for (uint32_t iPt = 0; iPt < numKeypoints; iPt++) {
        const cv::Point& keypoint = keypoints[iPt];
        const cv::Rect patchRoi(keypoint.x - templateSize.width/2,
                keypoint.y - templateSize.height/2,
                templateSize.width, templateSize.height);
        const cv::Mat patch = doSafeCrop(image, patchRoi);

        featureVectors[iPt] = featureExtractor.execute(patch);
    }

    return featureVectors;
}

void SceneFeatureExtractor::validateImage(const cv::Mat& image) const {
    shared::VALIDATE_ARGUMENT(!image.empty(),
            "SceneFeatureExtractor: image  "
            "must not be empty");
    shared::VALIDATE_ARGUMENT(image.type() == CV_32FC1,
            "SceneFeatureExtractor: image "
            "must be of type CV_32FC1");
}

void SceneFeatureExtractor::validateKeypoints(const std::vector<cv::Point>& keypoints,
        const cv::Mat& image) const {
    const cv::Rect imageRoi(0, 0, image.cols, image.rows);
    for (const cv::Point& keypoint : keypoints) {
        shared::VALIDATE_ARGUMENT(imageRoi.contains(keypoint),
                "SceneFeatureExtractor: Keypoint tried to access "
                "coordinate out of range");
    }
}

cv::Mat SceneFeatureExtractor::doSafeCrop(const cv::Mat& image, const cv::Rect& imageRoi) const {
    const cv::Size imageSize = image.size();
    const cv::Rect imageBoundary(0, 0, imageSize.width, imageSize.height);

    // We have two code paths based on whether or not the imageRoi is strictly inside
    // the image boundaries.
    const bool roiIsInside = (imageBoundary.contains(imageRoi.tl()) &&
            imageBoundary.contains(imageRoi.br()));

    cv::Mat crop;
    if (roiIsInside) {
        // Efficient shallow crop around imageRoi
        crop = image(imageRoi);
    } else {
        // Deep crop padding the out of bounds region with zeros
        crop = cv::Mat::zeros(imageRoi.height, imageRoi.width, CV_32FC1);
        const cv::Rect clampedImageRoi = (imageRoi & imageBoundary); 
        const cv::Rect cropRoi = clampedImageRoi - imageRoi.tl();
        image(clampedImageRoi).copyTo(crop(cropRoi));
    }

    return crop;
}

