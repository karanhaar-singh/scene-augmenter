#include "SceneAugmenter.hpp"
#include "SceneAugmenterPri.hpp"

SceneAugmenter::SceneAugmenter(const std::string& modelPath) :
        sceneAugmenterPri(std::make_shared<SceneAugmenterPri>(modelPath)) {};

void SceneAugmenter::setSourceImage(const cv::Mat& newSourceImage) {
    sceneAugmenterPri->setSourceImage(newSourceImage);
}

void SceneAugmenter::setReplacementImage(const cv::Mat& newReplacementImage) {
    sceneAugmenterPri->setReplacementImage(newReplacementImage);
}

cv::Mat SceneAugmenter::execute(const cv::Mat& targetImage) const {
    return sceneAugmenterPri->execute(targetImage);
}

