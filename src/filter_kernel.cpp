#include "filter_kernel.h"

FilterKernel::~FilterKernel()
{ }

float FilterKernel::at(int col, int row) {
    return kernel_values.at(row * ksize + col);
}

// TODO: exception should be thrown
float FilterKernel::at1D(int idx) {
    return 0.0f;
}

bool FilterKernel::isSeparableInto1D() {
    return false;
}

std::vector<float> FilterKernel::getValues() {
    return kernel_values;
}

unsigned short FilterKernel::getSize() {
    return ksize;
}