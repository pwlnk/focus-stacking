#include "images_utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdexcept>

namespace images_utils {

    void readImagesFromDirToFocusStack(std::string images_dir_path, FocusStack &focus_stack) {
        if (!dirExists(images_dir_path)) {
            throw std::invalid_argument("directory does not exist");
        }

        std::vector<cv::String> filenames;
        cv::glob(cv::String(images_dir_path), filenames);
        std::sort(filenames.begin(), filenames.end());

        for (auto& filename : filenames) {
            cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
            std::cout << "reading image: " << filename << std::endl;
            focus_stack.addImage(cvMat2Matrix(image));
        }
    }

    void showImage(std::string window_name, Matrix<uint8_t> image) {
        cv::Mat cv_image = images_utils::matrix2CvMat(image);
        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
        cv::imshow(window_name, cv_image);
    }

    void storeImageOnDisk(std::string filepath, Matrix<uint8_t> image) {
        cv::Mat cv_image = images_utils::matrix2CvMat(image);
        cv::imwrite(filepath, cv_image);
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

    bool dirExists(std::string dir_path) {
        struct stat dir_info;
        return stat(dir_path.c_str(), &dir_info) == 0 && S_ISDIR(dir_info.st_mode);
    }

}