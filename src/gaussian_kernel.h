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
        float at(int col, int row);
        float at1D(int idx);
        bool isSeparableInto1D();
};