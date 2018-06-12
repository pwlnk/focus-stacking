#pragma once

#include <vector>
#include <stdint.h>
#include <tuple>
#include "matrix.h"

class FocusStack {
    private:
        std::vector<Matrix<uint8_t>> images_stack;
        Matrix<uint8_t> all_in_focus_image;
        Matrix<uint8_t> depth_map;

        void computeLaplacianOfGaussianStack(std::vector<Matrix<float>>& log_stack);
        void computeAllInFocusAndDepthMap(std::vector<Matrix<float>>& log_stack,
                                          const int bg_threshold);
        int maxLoGIdx(int col, int row, std::vector<Matrix<float>> &log_stack,
                      const int bg_threshold);

    public:
        FocusStack();

        void addImage(Matrix<uint8_t> image);
        void computeAllInFocusAndDepthMap(const int bg_threshold = 70);

        Matrix<uint8_t> getAllInFocusImage() const;
        Matrix<uint8_t> getDepthMap() const;
};