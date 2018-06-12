#pragma once

#include "image_filter.h"

class LaplacianKernel : public FilterKernel {
    public:
        explicit LaplacianKernel(unsigned short kernel_size = 7);
        ~LaplacianKernel() override = default;
};