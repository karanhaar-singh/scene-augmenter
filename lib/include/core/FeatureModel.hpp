/**
 * These structs define a configuration for feature extraction.
 */

#pragma once

#include <fstream>
#include <vector>

#include "opencv2/core.hpp"

namespace core {

/** 
 * Contains information of the FeatureModel at a specific orientation.
 */
struct FeatureModelAtAngle {
    // Algorithm: BRIEF point pairs
    std::vector<cv::Point> points1;
    std::vector<cv::Point> points2;

    FeatureModelAtAngle() {};
    FeatureModelAtAngle(const std::vector<cv::Point>& _points1,
            const std::vector<cv::Point>& _points2) :
            points1(_points1), points2(_points2) {};
};

struct FeatureModel {
    // Expected size of the image to extract features from
    cv::Size templateSize;
    // Number of model orientations between 0 and 2pi radians
    uint32_t numAngleBuckets;
    // All orientation specific feature models
    std::vector<FeatureModelAtAngle> featureModelAtAngles;
public:
    FeatureModel() {};
    FeatureModel(const cv::Size& _templateSize, uint32_t _numAngleBuckets,
            const std::vector<FeatureModelAtAngle>& _featureModelAtAngles) :
            templateSize(_templateSize), numAngleBuckets(_numAngleBuckets),
            featureModelAtAngles(_featureModelAtAngles) {};

    /** 
     * Build a new FeatureModel using the given model path.
     *
     * @param modelPath Input file path to the FeatureModel
     */
    FeatureModel(const std::string& modelPath);

    /** 
     * Writes the FeatureModel to the specified modelPath on disk.
     *
     * @param modelPath Output file path to the FeatureModel
     */
    void write(const std::string& modelPath) const;
private:
    /** 
     * Reads a cv::Size object from the given stream.
     *
     * @param inputStream Input stream to read from
     * @return The cv::Size object
     */
    cv::Size readSize(std::ifstream& inputStream) const;
    /** 
     * Reads all FeatureModelAtAngle objects from the given stream
     *
     * @param inputStream Input stream to read from
     * @param _templateSize Image template size
     * @param _numAngleBuckets Number of orientations between
     *        0 and 2pi radians
     * @return The FeatureModelAtAngle objects
     */
    std::vector<FeatureModelAtAngle> readFeatureModelAtAngles(
            std::ifstream& inputStream, const cv::Size& _templateSize,
            uint32_t _numAngleBuckets) const;
    /** 
     * Reads cv::Point objects from the given stream.
     *
     * @param _templateSize Image template size
     * @return The cv::Point objects
     */
    std::vector<cv::Point> readPoints(std::ifstream& inputStream,
            const cv::Size& _templateSize) const;

    /** 
     * Writes a cv::Size object to the given stream.
     *
     * @param outputStream Output stream to write to
     * @param size The cv::Size object to write
     */
    void writeSize(std::ofstream& outputStream, 
            const cv::Size& size) const;
    /** 
     * Writes FeatureModelAtAngle objects to the given stream.
     *
     * @param outputStream Output stream to write to
     * @param _featureModelAtAngles The FeatureModelAtAngle objects to write
     */
    void writeFeatureModelAtAngles(std::ofstream& outputStream, 
            const std::vector<FeatureModelAtAngle>& _featureModelAtAngles) const;
    /** 
     * Writes cv::Point objects to the given stream.
     *
     * @param outputStream Output stream to write to
     * @param points The cv::Point objects to write
     */
    void writePoints(std::ofstream& outputStream, 
            const std::vector<cv::Point>& points) const;
};

}

