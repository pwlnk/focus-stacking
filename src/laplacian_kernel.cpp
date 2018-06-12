#include "laplacian_kernel.h"

LaplacianKernel::LaplacianKernel(unsigned short kernel_size)
{
    ksize = kernel_size;
    kernel_values = std::vector<float>(kernel_size * kernel_size, 1.0f);
    int kernel_center_idx = (kernel_size - 1) / 2;
    kernel_values.at(kernel_size * kernel_center_idx + kernel_center_idx) = - kernel_size * kernel_size + 1;
}