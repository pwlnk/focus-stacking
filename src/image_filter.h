#pragma once

#include <stdint.h>
#include <iostream>
#include "matrix.h"
#include "filter_kernel.h"
#include "convolution_strategy.h"
#include "serial_convolution_strategy.h"
#include "parallel_convolution_strategy.h"

template <typename TInput, typename TOutput>
class ImageFilter {
    private:
        FilterKernel& kernel;
        ConvolutionStrategy<TInput, TOutput>* convolution_strategy;

    public:
        ImageFilter(FilterKernel& kernel);
        ~ImageFilter();

        Matrix<TOutput> convolution(Matrix<TInput>& image);
};

template <typename TInput, typename TOutput>
ImageFilter<TInput, TOutput>::ImageFilter(FilterKernel& kernel) :
kernel(kernel)
{
//    convolution_strategy = new SerialConvolutionStrategy<TInput, TOutput>();
    convolution_strategy = new ParallelConvolutionStrategy<TInput, TOutput>();
}

template <typename TInput, typename TOutput>
ImageFilter<TInput, TOutput>::~ImageFilter() {
    delete convolution_strategy;
};

template <typename TInput, typename TOutput>
Matrix<TOutput> ImageFilter<TInput, TOutput>::convolution(Matrix<TInput>& image) {
    Shape result_image_shape = image.getShape();
    Matrix<TOutput> filtered_image(result_image_shape);

    if (kernel.isSeparableInto1D()) {
        convolution_strategy->convolution1D(image, filtered_image, kernel);
    }
    else {
        convolution_strategy->convolution2D(image, filtered_image, kernel);
    }

    return filtered_image;
}