#pragma once

#include "image_filter.h"

class LaplacianKernel : public FilterKernel {
    public:
        LaplacianKernel(unsigned short kernel_size = 7);
        ~LaplacianKernel();
};