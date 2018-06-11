#pragma once

#include <iostream>
#include "matrix.h"

class GrayscaleConverter {
    private:
        GrayscaleConverter();

    public:
        static Matrix<uint8_t> convertToGrayscale(const Matrix<uint8_t>& color_image);
};