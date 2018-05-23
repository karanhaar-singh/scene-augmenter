#include "core/FeatureModel.hpp"

#include "shared/Definitions.hpp"

#include "core/Definitions.hpp"

namespace core {

FeatureModel::FeatureModel(const std::string& modelPath) {
    std::ifstream inputStream(modelPath, std::ios::in | std::ios::binary);
    shared::VALIDATE_ARGUMENT(inputStream.is_open(),
            "core::FeatureModel: Invalid modelPath: " + modelPath);

    const cv::Size _templateSize = readSize(inputStream);
    uint32_t _numAngleBuckets;
    inputStream.read((char*)&_numAngleBuckets, sizeof(uint32_t));
    const std::vector<FeatureModelAtAngle> _featureModelAtAngles =
            readFeatureModelAtAngles(inputStream, _templateSize, _numAngleBuckets);

    // Check that we are at exactly the end of the file, we do this by making sure
    // eofbit is NOT set immediately after reading the last byte but set after peeking
    // to the next byte which should not exit
    shared::VALIDATE_ARGUMENT(!inputStream.eof(),
            "core::FeatureModel: Corrupted model: Too few bytes");
    inputStream.peek();
    shared::VALIDATE_ARGUMENT(inputStream.eof(),
            "core::FeatureModel: Corrupted model: Too many bytes");

    // Populate data members at the end to ensure exception
    templateSize = _templateSize;
    numAngleBuckets = _numAngleBuckets;
    featureModelAtAngles = _featureModelAtAngles;
}

void FeatureModel::write(const std::string& modelPath) const {
    std::ofstream outputStream(modelPath, std::ios::out | std::ios::binary);

    writeSize(outputStream, templateSize);
    outputStream.write((char*)&numAngleBuckets, sizeof(uint32_t));
    writeFeatureModelAtAngles(outputStream, featureModelAtAngles); 
}

cv::Size FeatureModel::readSize(std::ifstream& inputStream) const {
    // Read cv::Size from the stream
    cv::Size _templateSize;
    inputStream.read((char*)&_templateSize.width, sizeof(int32_t));
    inputStream.read((char*)&_templateSize.height, sizeof(int32_t));

    // Validate read object
    const bool templateIsValid = (_templateSize.area() > 0);
    shared::VALIDATE_ARGUMENT(templateIsValid,
            "core::FeatureModel: Corrupted model: Invalid template size");

    return _templateSize;
}

std::vector<FeatureModelAtAngle> FeatureModel::readFeatureModelAtAngles(
        std::ifstream& inputStream, const cv::Size& _templateSize,
        uint32_t _numAngleBuckets) const {
    std::vector<FeatureModelAtAngle> _featureModelAtAngles;
    for (uint32_t iBucket = 0; iBucket < _numAngleBuckets; iBucket++) {
        const std::vector<cv::Point> points1 = readPoints(inputStream, _templateSize);
        const std::vector<cv::Point> points2 = readPoints(inputStream, _templateSize);
        _featureModelAtAngles.emplace_back(points1, points2);
    }

    return _featureModelAtAngles;
}

std::vector<cv::Point> FeatureModel::readPoints(std::ifstream& inputStream,
        const cv::Size& _templateSize) const {
    // Build ROI from template for error checking
    const cv::Rect templateRoi(0, 0, _templateSize.width, _templateSize.height);

    // Read points one-by-one
    std::vector<cv::Point> points;
    for (uint32_t iPoint = 0; iPoint < NUM_BRIEF_BITS; iPoint++) {
        cv::Point point;
        inputStream.read((char*)&point.x, sizeof(int32_t));
        inputStream.read((char*)&point.y, sizeof(int32_t));

        shared::VALIDATE_ARGUMENT(templateRoi.contains(point),
                "core::FeatureModel: Corrupted model: Point out of template range");

        points.push_back(point);
    }

    return points;
}

void FeatureModel::writeSize(std::ofstream& outputStream, 
        const cv::Size& size) const {
    outputStream.write((char*)&size.width, sizeof(int32_t));
    outputStream.write((char*)&size.height, sizeof(int32_t));
}

void FeatureModel::writeFeatureModelAtAngles(std::ofstream& outputStream, 
        const std::vector<FeatureModelAtAngle>& _featureModelAtAngles) const {
    for (const FeatureModelAtAngle& featureModelAtAngle : _featureModelAtAngles) {
        writePoints(outputStream, featureModelAtAngle.points1);
        writePoints(outputStream, featureModelAtAngle.points2);
    }
}

void FeatureModel::writePoints(std::ofstream& outputStream, 
        const std::vector<cv::Point>& points) const {
    for (const cv::Point& point : points) {
        outputStream.write((char*)&point.x, sizeof(int32_t));
        outputStream.write((char*)&point.y, sizeof(int32_t));
    }
}

}

