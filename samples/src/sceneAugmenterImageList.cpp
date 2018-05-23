/**
 * Service that will run scene augmentation on a list of images.
 */

#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "SceneAugmenter.hpp"


// Helper function headers
std::vector<std::string> readFileLines(const std::string& inputFilePath);


int main(int argc, char* argv[]) {
    // Read and parse arguments
    if (argc != 6) {
        std::cout << "Usage: <service name> <input source image path> " <<
                "<input replacement image path> <input target image list path> " <<
                "<model path> <output folder>" << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::string sourceImagePath(argv[1]);
    const std::string replacementImagePath(argv[2]);
    const std::string targetImageListPath(argv[3]);
    const std::string modelPath(argv[4]);
    const std::string outputFolder(argv[5]);

    // Prepare output folder structure
    mkdir(outputFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    
    // Load source and replacement images
    const cv::Mat sourceImage = cv::imread(sourceImagePath);
    const cv::Mat replacementImage = cv::imread(replacementImagePath);
    // Load list of target images
    const std::vector<std::string> targetImageList = readFileLines(targetImageListPath);


    // Run preparation pipeline
    SceneAugmenter sceneAugmenter(modelPath);
    sceneAugmenter.setSourceImage(sourceImage);
    sceneAugmenter.setReplacementImage(replacementImage);

    // Run scene augmenter pipeline
    for (uint32_t iFrame = 0; iFrame < targetImageList.size(); iFrame++) {
        const std::string targetImagePath = targetImageList[iFrame];
        std::cout << targetImagePath << std::endl;

        const cv::Mat targetImage = cv::imread(targetImagePath);
        const cv::Mat augmentedImage = sceneAugmenter.execute(targetImage);

        // Write to disk
        cv::imwrite(outputFolder + "/" + std::to_string(iFrame) + ".png", augmentedImage);
    }

    exit(EXIT_SUCCESS);
}

/**
 * Helper function that parses a vector of lines from a text file.
 */
std::vector<std::string> readFileLines(const std::string& inputFilePath) {
    std::string fileLine;
    std::vector<std::string> fileLines;
    std::ifstream fileHandle(inputFilePath);
    while (std::getline(fileHandle, fileLine)) {
        fileLines.push_back(fileLine);
    }

    return fileLines;
}

