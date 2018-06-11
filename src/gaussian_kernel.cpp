#include "gaussian_kernel.h"

GaussianKernel::GaussianKernel() :
ksize(7), sigma(5.0f)
{
    kernel_values = {
            0.016641,	0.018385,	0.019518,	0.019911,	0.019518,	0.018385,	0.016641,
            0.018385,	0.020312,	0.021564,	0.021998,	0.021564,	0.020312,	0.018385,
            0.019518,	0.021564,	0.022893,	0.023354,	0.022893,	0.021564,	0.019518,
            0.019911,	0.021998,	0.023354,	0.023824,	0.023354,	0.021998,	0.019911,
            0.019518,	0.021564,	0.022893,	0.023354,	0.022893,	0.021564,	0.019518,
            0.018385,	0.020312,	0.021564,	0.021998,	0.021564,	0.020312,	0.018385,
            0.016641,	0.018385,	0.019518,	0.019911,	0.019518,	0.018385,	0.016641};

    kernel_values_1D = {
            0.129001,	0.142521,	0.151303,	0.15435,	0.151303,	0.142521,	0.129001};
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

float GaussianKernel::at1D(size_t idx) {
    return kernel_values_1D.at(idx);
}

bool GaussianKernel::isSeparableInto1D() {
    return true;
}