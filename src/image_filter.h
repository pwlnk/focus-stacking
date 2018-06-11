#pragma once

#include <stdint.h>
#include "matrix.h"
#include "filter_kernel.h"

class ImageFilter {
    private:
        ImageFilter();
        static void convolution2D(Matrix<uint8_t>& image, Matrix<uint8_t>& filtered_image, FilterKernel& kernel);
        static void convolution1D(Matrix<uint8_t>& image, Matrix<uint8_t>& filtered_image, FilterKernel& kernel);

    public:
        static Matrix<uint8_t> convolution(Matrix<uint8_t>& image, FilterKernel& kernel);
};