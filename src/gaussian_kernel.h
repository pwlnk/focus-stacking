#pragma once

#include "filter_kernel.h"

class GaussianKernel : public FilterKernel {
    private:
        unsigned short ksize;
        float sigma;
        std::vector<float> kernel_values;
        std::vector<float> kernel_values_1D;

    public:
        GaussianKernel();
        ~GaussianKernel();

        std::vector<float> getValues();
        unsigned short getSize();
        float at(size_t col, size_t row);
        float at1D(size_t idx);
        bool isSeparableInto1D();
};