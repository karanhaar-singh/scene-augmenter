/**
 * This struct provides a simple interface of the concept of storing keypoint matches between
 * two images.
 */

#pragma once

#include <vector>

#include "opencv2/core.hpp"

#include "Definitions.hpp"

struct MatchingPoints {
    // Points in the first (source) image
    std::vector<cv::Point> fromPts;
    // Respective points in the second (target) image
    std::vector<cv::Point> toPts;
public:
    /**
     * Simple constructor that validates and directly assigns the two data members
     * from the arguments.
     * 
     * @param _fromPts The new fromPts
     * @param _toPts The new toPts
     */
    MatchingPoints(const std::vector<cv::Point>& _fromPts,
            const std::vector<cv::Point>& _toPts);
    /**
     * Simple constructor that validates the two vectors of keypoints and the associated
     * correspondences then populates the two data members.
     * 
     * @param sourceKeypoints The list of keypoints detected in the first (source) image
     * @param targetKeypoints The list of keypoints detected in the second (target) image
     * @param correspondences The correspondences between the aforementioned lists of keypoints
     */
    MatchingPoints(const std::vector<cv::Point>& sourceKeypoints,
            const std::vector<cv::Point>& targetKeypoints,
            const Correspondences& correspondences);
    MatchingPoints() {};
private:
    /**
     * Validates the internal integrity of the two vectors of keypoints and the associated
     * correspondences.
     * 
     * @param sourceKeypoints The list of keypoints detected in the first (source) image
     * @param targetKeypoints The list of keypoints detected in the second (target) image
     * @param correspondences The correspondences between the aforementioned lists of keypoints
     */
    void validateCorrespondences(
            const std::vector<cv::Point>& sourceKeypoints,
            const std::vector<cv::Point>& targetKeypoints,
            const Correspondences& correspondences) const;
};

