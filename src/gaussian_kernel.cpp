#include "gaussian_kernel.h"

GaussianKernel::GaussianKernel() :
ksize(7), sigma(5.0f)
{
//    kernel_values = {1,  4,  7,  4, 1,
//                     4, 16, 26, 16, 4,
//                     7, 26, 41, 26, 7,
//                     4, 16, 26, 16, 4,
//                     1,  4,  7,  4, 1};
//
//    for (auto& value : kernel_values) {
//        value *= 1.0f / 273.0f;
//    }

    kernel_values = {
        0.016641,	0.018385,	0.019518,	0.019911,	0.019518,	0.018385,	0.016641,
        0.018385,	0.020312,	0.021564,	0.021998,	0.021564,	0.020312,	0.018385,
        0.019518,	0.021564,	0.022893,	0.023354,	0.022893,	0.021564,	0.019518,
        0.019911,	0.021998,	0.023354,	0.023824,	0.023354,	0.021998,	0.019911,
        0.019518,	0.021564,	0.022893,	0.023354,	0.022893,	0.021564,	0.019518,
        0.018385,	0.020312,	0.021564,	0.021998,	0.021564,	0.020312,	0.018385,
        0.016641,	0.018385,	0.019518,	0.019911,	0.019518,	0.018385,	0.016641};
}

GaussianKernel::~GaussianKernel()
{ }

std::vector<float> GaussianKernel::getValues() {
    return kernel_values;
}

unsigned short GaussianKernel::getSize() {
    return ksize;
}

float GaussianKernel::at(size_t col, size_t row) {
    return kernel_values.at(row * ksize + col);
}