#pragma once

#include "matrix.h"
#include "filter_kernel.h"

template <typename TInput, typename TOutput>
class ConvolutionStrategy {
    public:
        virtual void convolution2D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel) = 0;
        virtual void convolution1D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel) = 0;
};