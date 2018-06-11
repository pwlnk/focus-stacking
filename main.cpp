#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#include "src/matrix.h"
#include "src/grayscale_converter.h"

// TODO: extract to separate utils class
Matrix<uint8_t> cvMat2Matrix(cv::Mat& opencv_matrix);
cv::Mat matrix2CvMat(Matrix<uint8_t>& matrix);
short channelBGR2RGB(short BGR_channel, short channels_num);

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
        cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
        std::cout << "reading image: " << filename << std::endl;
        images.push_back(cvMat2Matrix(image));
    }

    Matrix<uint8_t> grayscale_image = GrayscaleConverter::convertToGrayscale(images[0]);

    cv::Mat image = matrix2CvMat(grayscale_image);
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0);


    return 0;
}

Matrix<uint8_t> cvMat2Matrix(cv::Mat& opencv_matrix) {
    Shape shape(opencv_matrix.cols, opencv_matrix.rows, opencv_matrix.channels());
    Matrix<uint8_t> matrix(shape);

    int channels = opencv_matrix.channels();

    for (size_t row = 0; row < matrix.getShape().y; row++) {
        uint8_t* opencv_matrix_row_pointer = opencv_matrix.ptr(row);
        for (size_t col = 0; col < matrix.getShape().x; col++) {
            for (size_t channel = 0; channel < channels; channel++) {
                matrix.at(col, row, channelBGR2RGB(channel, channels)) = opencv_matrix_row_pointer[col * channels + channel];
            }
        }
    }

    return matrix;
}

cv::Mat matrix2CvMat(Matrix<uint8_t>& matrix) {
    int channels = matrix.getShape().z;
    cv::Mat opencv_matrix;

    switch (channels) {
        case 1 :
            opencv_matrix = cv::Mat(matrix.getShape().y, matrix.getShape().x, CV_8UC1);
            break;
        case 3 :
            opencv_matrix = cv::Mat(matrix.getShape().y, matrix.getShape().x, CV_8UC3);
            break;
    }


    for (size_t row = 0; row < matrix.getShape().y; row++) {
        uint8_t* opencv_matrix_row_pointer = opencv_matrix.ptr(row);
        for (size_t col = 0; col < matrix.getShape().x; col++) {
            for (size_t channel = 0; channel < channels; channel++) {
                opencv_matrix_row_pointer[col * channels + channel] = matrix.at(col, row, channelBGR2RGB(channel, channels));
            }
        }
    }

    return opencv_matrix;
}

short channelBGR2RGB(short BGR_channel, short channels_num) {
    return channels_num - BGR_channel - 1;
}