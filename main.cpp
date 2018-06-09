#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#include "src/matrix.h"

// TODO: extract to separate utils class
Matrix<uint8_t> cvMat2Matrix(cv::Mat& opencv_matrix);

int main(int argc, char* argv[]) {

    const char* DEFAULT_IMG_DIR = "input/";
    const char* img_dir;

    std::vector<Matrix<uint8_t>> images;

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
        images.push_back(cvMat2Matrix(image));
    }

    return 0;
}

Matrix<uint8_t> cvMat2Matrix(cv::Mat& opencv_matrix) {
    Shape shape(opencv_matrix.cols, opencv_matrix.rows, opencv_matrix.channels());
    Matrix<uint8_t> matrix(shape);

    for (size_t i = 0; i < shape.x * shape.y * shape.z; i++) {
        matrix[i] = opencv_matrix.data[i];
    }

    return matrix;
}