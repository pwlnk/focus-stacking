#pragma once

#include <stdint.h>
#include "matrix.h"
#include "filter_kernel.h"

class ImageFilter {
    private:
        ImageFilter();
        static short getSamePaddingValue(short kernel_size);

    public:
        static Matrix<uint8_t> convolution(Matrix<uint8_t>& image, FilterKernel& kernel);
};