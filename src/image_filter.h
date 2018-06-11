#pragma once

#include <stdint.h>
#include "matrix.h"
#include "filter_kernel.h"

class ImageFilter {
    private:
        FilterKernel& kernel;

        void convolution2D(Matrix<uint8_t>& image, Matrix<uint8_t>& filtered_image);
        void convolution1D(Matrix<uint8_t>& image, Matrix<uint8_t>& filtered_image);

    public:
        ImageFilter(FilterKernel& kernel);

        Matrix<uint8_t> convolution(Matrix<uint8_t>& image);
};