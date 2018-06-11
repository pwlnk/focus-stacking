#pragma once

#include "image_filter.h"

class LaplacianKernel : public FilterKernel {
    private:
        unsigned short ksize;
        std::vector<float> kernel_values;

    public:
        LaplacianKernel();
        ~LaplacianKernel();

        std::vector<float> getValues();
        unsigned short getSize();
        float at(int col, int row);
        float at1D(int idx);
        bool isSeparableInto1D();
};