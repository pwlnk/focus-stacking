#include "laplacian_kernel.h"

LaplacianKernel::LaplacianKernel() :
ksize(3)
{
    kernel_values = {0, 1, 0, 1, -4, 1, 0, 1, 0};
}

LaplacianKernel::~LaplacianKernel()
{ }

std::vector<float> LaplacianKernel::getValues() {
    return kernel_values;
}

unsigned short LaplacianKernel::getSize() {
    return ksize;
}

float LaplacianKernel::at(int col, int row) {
    return kernel_values.at(row * ksize + col);
}

// TODO: exception should be thrown
float LaplacianKernel::at1D(int idx) {
    return 0.0f;
}

bool LaplacianKernel::isSeparableInto1D() {
    return false;
}