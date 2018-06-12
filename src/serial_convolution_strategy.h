#pragma once

#include "convolution_strategy.h"

template <typename TInput, typename TOutput>
class SerialConvolutionStrategy : public ConvolutionStrategy<TInput, TOutput> {
    private:
        float convolution2DStep(int col, int row, int channel, Matrix<TInput>& image,
                                Matrix<TOutput>& filtered_image, FilterKernel& kernel);
        float convolution1DStep(int col, int row, int channel, int kernel_direction, Matrix<TInput>& image,
                                Matrix<TOutput>& filtered_image, Matrix<TOutput>& after_first_pass_image, FilterKernel& kernel);

    public:
        void convolution2D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel);
        void convolution1D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel);
};

template <typename TInput, typename TOutput>
void SerialConvolutionStrategy<TInput, TOutput>::convolution2D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel) {
    for (int col = 0; col < image.getShape().x; col++) {
        for (int row = 0; row < image.getShape().y; row++) {
            for (int channel = 0; channel < image.getShape().z; channel++) {
                filtered_image.at(col, row, channel) = convolution2DStep(col, row, channel, image, filtered_image, kernel);
            }
        }
    }
}

template <typename TInput, typename TOutput>
float SerialConvolutionStrategy<TInput, TOutput>::convolution2DStep(int col, int row, int channel, Matrix<TInput>& image,
                                                                    Matrix<TOutput>& filtered_image, FilterKernel& kernel) {
    float convolution_step_value = 0.0f;
    for (int kernel_col = 0; kernel_col < kernel.getSize(); kernel_col++) {
        for (int kernel_row = 0; kernel_row < kernel.getSize(); kernel_row++) {
            int image_col_idx = col - (kernel.getSize() - 1) / 2 + kernel_col;
            int image_row_idx = row - (kernel.getSize() - 1) / 2 + kernel_row;

            uint8_t image_value = 0;
            if (image_col_idx >= 0 && image_col_idx < image.getShape().x &&
                image_row_idx >= 0 && image_row_idx < image.getShape().y) {
                image_value = image.at(image_col_idx, image_row_idx, channel);
            }

            convolution_step_value += kernel.at(kernel_col, kernel_row) * image_value;
        }
    }

    return convolution_step_value;
}

template <typename TInput, typename TOutput>
void SerialConvolutionStrategy<TInput, TOutput>::convolution1D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel) {
    Matrix<TOutput> after_first_pass_image(image.getShape());

    for (int kernel_direction = 0; kernel_direction < 2; kernel_direction++) {
        for (int row = 0; row < image.getShape().y; row++) {
            for (int col = 0; col < image.getShape().x; col++) {
                for (int channel = 0; channel < image.getShape().z; channel++) {
                    float convolution_step_value = convolution1DStep(col, row, channel, kernel_direction, image, filtered_image, after_first_pass_image, kernel);
                    filtered_image.at(col, row, channel) = convolution_step_value;

                    // TODO: get rid of magic number
                    if (kernel_direction == 0) {
                        after_first_pass_image.at(col, row, channel) = convolution_step_value;
                    }
                }
            }
        }
    }
}

template <typename TInput, typename TOutput>
float SerialConvolutionStrategy<TInput, TOutput>::convolution1DStep(int col, int row, int channel, int kernel_direction, Matrix<TInput>& image,
                                                                    Matrix<TOutput>& filtered_image, Matrix<TOutput>& after_first_pass_image, FilterKernel& kernel) {
    float convolution_step_value = 0;
    for (int kernel_idx = 0; kernel_idx < kernel.getSize(); kernel_idx++) {
        int image_col_idx = kernel_direction == 0 ? col - (kernel.getSize() - 1) / 2 + kernel_idx : col;
        int image_row_idx = kernel_direction == 1 ? row - (kernel.getSize() - 1) / 2 + kernel_idx : row;

        float image_value = 0.0;
        if (image_col_idx >= 0 && image_col_idx < image.getShape().x &&
            image_row_idx >= 0 && image_row_idx < image.getShape().y) {
            image_value = kernel_direction == 0 ?
                          image.at(image_col_idx, image_row_idx, channel) :
                          after_first_pass_image.at(image_col_idx, image_row_idx, channel);
        }

        convolution_step_value += kernel.at1D(kernel_idx) * image_value;
    }

    return convolution_step_value;
}