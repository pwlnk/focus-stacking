#pragma once

#include "filter_kernel.h"

class GaussianKernel : public FilterKernel {
    private:
        unsigned short ksize;
        float sigma;
        std::vector<float> kernel_values;

    public:
        GaussianKernel();
        ~GaussianKernel();

        std::vector<float> getValues();
        unsigned short getSize();
        float at(size_t col, size_t row);
};