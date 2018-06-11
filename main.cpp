#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <cmath>

#include <cstdio>
#include <ctime>

#include "src/matrix.h"
#include "src/grayscale_converter.h"
#include "src/image_filter.h"
#include "src/gaussian_kernel.h"
#include "src/laplacian_kernel.h"

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

    std::vector<Matrix<float>> laplacian_images;

    for (auto image : images) {
        std::cout << "processing image" << std::endl;
        Matrix<uint8_t> grayscale_image = GrayscaleConverter::convertToGrayscale(image);

        GaussianKernel gaussian_kernel;
        ImageFilter<uint8_t, uint8_t> gaussian_filter(gaussian_kernel);
        Matrix<uint8_t> blurred_image = gaussian_filter.convolution(grayscale_image);

        LaplacianKernel laplacian_kernel;
        ImageFilter<uint8_t, float> laplacian_filter(laplacian_kernel);
        Matrix<float> laplacian_image = laplacian_filter.convolution(blurred_image);

        laplacian_images.push_back(laplacian_image);
    }

    Shape images_shape = laplacian_images[0].getShape();
    Matrix<uint8_t> depth_map(images_shape);
    Matrix<uint8_t> all_in_focus_image(images_shape.x, images_shape.y, 3);
    for (int row = 0; row < images_shape.y; row++) {
        for (int col = 0; col < images_shape.x; col++) {
            int max_idx = 0;
            float max_image_value = 0;

            for (int image_idx = 0; image_idx < laplacian_images.size(); image_idx++) {
                if (abs(laplacian_images.at(image_idx).at(col, row, 0)) > max_image_value) {
                    max_idx = image_idx;
                    max_image_value = abs(laplacian_images.at(image_idx).at(col, row, 0));
                }
            }

            depth_map.at(col, row, 0) = max_idx * (255 / laplacian_images.size());
            all_in_focus_image.at(col, row, 0) = images.at(max_idx).at(col, row, 0);
            all_in_focus_image.at(col, row, 1) = images.at(max_idx).at(col, row, 1);
            all_in_focus_image.at(col, row, 2) = images.at(max_idx).at(col, row, 2);
        }
    }

    cv::Mat image = matrix2CvMat(depth_map);
    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::imwrite("../../depth_map.jpg", image);

    cv::Mat focus_image = matrix2CvMat(all_in_focus_image);
    cv::namedWindow("Focus Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Focus Image", focus_image);
    cv::imwrite("../../focus_image.jpg", focus_image);

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