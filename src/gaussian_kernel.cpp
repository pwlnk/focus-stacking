#include "gaussian_kernel.h"

#include <cmath>
#include <numeric>
#include <algorithm>

GaussianKernel::GaussianKernel(unsigned short kernel_size, float sigma) :
sigma(sigma)
{
    ksize = kernel_size;
    generate2DKernel(kernel_size, sigma);
    generate1DKernel(kernel_size, sigma);
}

void GaussianKernel::generate2DKernel(unsigned short kernel_size, float sigma) {
    int kernel_reach = (kernel_size - 1) / 2;
    kernel_values.resize(kernel_size * kernel_size);

    for (int x = -kernel_reach; x <= kernel_reach; x++) {
        for (int y = -kernel_reach; y <= kernel_reach; y++) {
            kernel_values.at((y + kernel_reach) * ksize + x + kernel_reach) = gaussianPDFSample2D(x, y, sigma);
        }
    }

    normalizeKernel(kernel_values);
}

void GaussianKernel::generate1DKernel(unsigned short kernel_size, float sigma) {
    int kernel_reach = (ksize - 1) / 2;
    kernel_values_1D.resize(ksize);

    for (int x = -kernel_reach; x <= kernel_reach; x++) {
        kernel_values_1D.at(x + kernel_reach) = gaussianPDFSample1D(x, sigma);
    }

    normalizeKernel(kernel_values_1D);
}

// TODO: point samples are not 100% accurate, these samples should be integrals over whole pixel
float GaussianKernel::gaussianPDFSample2D(float x, float y, float sigma, float amplitude) {
    float center_x = 0; float center_y = 0;
    float exp_arg =  (std::pow(x - center_x, 2) + std::pow(y - center_y, 2)) / (2.0f * std::pow(sigma, 2));
    return amplitude * std::exp(-exp_arg);
}

float GaussianKernel::gaussianPDFSample1D(float x, float sigma, float amplitude) {
    float mean = 0;
    float exp_arg = std::pow((x - mean) / sigma, 2);
    return amplitude * std::exp(-0.5 * exp_arg);
}

void GaussianKernel::normalizeKernel(std::vector<float>& kernel) {
    float kernel_sum = std::accumulate(kernel.begin(), kernel.end(), 0.0f);
    std::transform(kernel.begin(), kernel.end(), kernel.begin(),
                   [kernel_sum](float kernel_value) -> float { return kernel_value / kernel_sum; });

}

float GaussianKernel::at1D(int idx) {
    return kernel_values_1D.at(idx);
}

bool GaussianKernel::isSeparableInto1D() {
    return true;
}