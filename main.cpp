#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#include "src/matrix.h"

int main(int argc, char* argv[]) {

    const char* DEFAULT_IMG_DIR = "input/";
    const char* img_dir;

    std::vector<cv::Mat> images;

    if (argc == 2) {
        img_dir = argv[1];
    } else {
        img_dir = DEFAULT_IMG_DIR;
    }

    std::vector<cv::String> filenames;
    cv::glob(cv::String(img_dir), filenames);
    std::sort(filenames.begin(), filenames.end());

    for (auto filename : filenames) {
        cv::Mat image = cv::imread(filename, 1);
        std::cout << "reading image: " << filename << std::endl;
        images.push_back(image);
    }

    return 0;
}