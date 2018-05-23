#include "core/homography/Builder.hpp"

#include <cmath>

#include "opencv2/imgproc.hpp"

namespace core {
namespace homography {

cv::Mat Builder::build(const std::vector<cv::Point>& fromPts,
        const std::vector<cv::Point>& toPts) {
    // Obtain the system of equations that defines the homography
    const cv::Mat A = buildA(fromPts, toPts);

    // One way to estimate the homography is to simply compute the eigenvector
    // associated with the smallest eigenvalue of the A matrix
    const cv::Mat AtA = A.t()*A;
    cv::Mat eigenvals, eigenvecs;
    cv::eigen(AtA, eigenvals, eigenvecs);
    const cv::Mat selectedEigenvec = eigenvecs.row(eigenvecs.rows - 1).clone();

    // Reshape to transformation matrix
    const cv::Mat homography = selectedEigenvec.reshape(0, TRANSFORM_MAT_SIDE);
    return homography;
}

cv::Mat Builder::buildA(const std::vector<cv::Point>& fromPts,
        const std::vector<cv::Point>& toPts) {
    cv::Mat A = cv::Mat::zeros(numRowsA, numColsA, CV_32F);
    for (uint32_t iMatch = 0; iMatch < MIN_BUILD_POINTS; iMatch++) {
        const float x = (float)fromPts[iMatch].x;
        const float y = (float)fromPts[iMatch].y;
        const float xp = (float)toPts[iMatch].x;
        const float yp = (float)toPts[iMatch].y;

        A.at<float>(2*iMatch, 3) = -x;
        A.at<float>(2*iMatch, 4) = -y;
        A.at<float>(2*iMatch, 5) = -1.0f;
        A.at<float>(2*iMatch, 6) = x*yp;
        A.at<float>(2*iMatch, 7) = y*yp;
        A.at<float>(2*iMatch, 8) = yp;

        A.at<float>(2*iMatch + 1, 0) = x;
        A.at<float>(2*iMatch + 1, 1) = y;
        A.at<float>(2*iMatch + 1, 2) = 1.0f;
        A.at<float>(2*iMatch + 1, 6) = -x*xp;
        A.at<float>(2*iMatch + 1, 7) = -y*xp;
        A.at<float>(2*iMatch + 1, 8) = -xp;
    }

    return A;
}

}
}

