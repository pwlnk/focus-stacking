#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

#include "focus_stack.h"

namespace images_utils {

    void readImagesFromDirToFocusStack(std::string images_dir_path, FocusStack &focus_stack);
    void showImage(std::string window_name, Matrix<uint8_t> image);
    void storeImageOnDisk(std::string filepath, Matrix<uint8_t> image);
    
    Matrix<uint8_t> cvMat2Matrix(cv::Mat& opencv_matrix);
    cv::Mat matrix2CvMat(Matrix<uint8_t>& matrix);
    
    short channelBGR2RGB(short BGR_channel, short channels_num);
    bool dirExists(std::string dir_path);

}