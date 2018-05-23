#include "MatchingPoints.hpp"

#include "shared/Definitions.hpp"

MatchingPoints::MatchingPoints(const std::vector<cv::Point>& _fromPts,
        const std::vector<cv::Point>& _toPts) {
    shared::VALIDATE_ARGUMENT(_fromPts.size() ==  _toPts.size(),
            "MatchingPoints: points vectors are not the same size");

    fromPts = _fromPts;
    toPts = _toPts;
}

MatchingPoints::MatchingPoints(const std::vector<cv::Point>& sourceKeypoints,
        const std::vector<cv::Point>& targetKeypoints,
        const Correspondences& correspondences) {
    validateCorrespondences(sourceKeypoints, targetKeypoints, correspondences);
    // Populate data members such that respective data member indices reference
    // a matching pair of points
    for (const auto& correspondence : correspondences) {
        fromPts.push_back(sourceKeypoints[correspondence.first]);
        toPts.push_back(targetKeypoints[correspondence.second]);
    }
};

void MatchingPoints::validateCorrespondences(
        const std::vector<cv::Point>& sourceKeypoints,
        const std::vector<cv::Point>& targetKeypoints,
        const Correspondences& correspondences) const {
    for (const auto& correspondence : correspondences) {
        const bool indicesAreValid = (correspondence.first < sourceKeypoints.size()) &&
                (correspondence.second < targetKeypoints.size());
        shared::VALIDATE_ARGUMENT(indicesAreValid,
                "MatchingPoints: correspondence references an invalid keypoint index");
    }
}

