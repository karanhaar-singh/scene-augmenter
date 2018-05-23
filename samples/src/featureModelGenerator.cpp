/**
 * Service that will build a new feature model.
 */

#include <iostream>
#include <string>

#include "core/FeatureModelGenerator.hpp"

static const cv::Size templateSize(13, 13);
static constexpr uint32_t numAngleBuckets = 48u;

int main(int argc, char* argv[]) {
    // Read and parse arguments
    if (argc != 2) {
        std::cout << "Usage: <service name> <output model file path>" << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::string outputModelPath(argv[1]);

    // Build feature model and save to disk
    core::FeatureModelGenerator featureModelGenerator(
            outputModelPath, templateSize, numAngleBuckets);
    featureModelGenerator.execute();

    exit(EXIT_SUCCESS);
}

