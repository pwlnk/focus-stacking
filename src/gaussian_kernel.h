#pragma once

#include "filter_kernel.h"

class GaussianKernel : public FilterKernel {
    private:
        float sigma;
        std::vector<float> kernel_values_1D;

        float gaussianPDFSample2D(float x, float y, float sigma, float amplitude = 1.0f);
        float gaussianPDFSample1D(float x, float sigma, float amplitude = 1.0f);
        void normalizeKernel(std::vector<float>& kernel);

        void generate2DKernel(float kernel_size, float sigma);
        void generate1DKernel(float kernel_size, float sigma);

    public:
        GaussianKernel(unsigned short kernel_size = 7, float sigma = 5.0f);
        ~GaussianKernel();

        float at1D(int idx);
        bool isSeparableInto1D();
};