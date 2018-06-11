#include <iostream>
#include "image_filter.h"

ImageFilter::ImageFilter(FilterKernel& kernel) :
kernel(kernel)
{ }

// TODO: result matrix cannot be uint (negative values possible)
Matrix<uint8_t> ImageFilter::convolution(Matrix<uint8_t>& image) {
    Shape result_image_shape = image.getShape();
    Matrix<uint8_t> filtered_image(result_image_shape);

    if (kernel.isSeparableInto1D()) {
        convolution1D(image, filtered_image);
    }
    else {
        convolution2D(image, filtered_image);
    }

    return filtered_image;
}

void ImageFilter::convolution2D(Matrix<uint8_t>& image, Matrix<uint8_t>& filtered_image) {
    for (int col = 0; col < image.getShape().x; col++) {
        for (int row = 0; row < image.getShape().y; row++) {
            for (int channel = 0; channel < image.getShape().z; channel++) {

                float convolution_step_value = 0;
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
                filtered_image.at(col, row, channel) = convolution_step_value;

            }
        }
    }
}

void ImageFilter::convolution1D(Matrix<uint8_t>& image, Matrix<uint8_t>& filtered_image) {
    Matrix<uint8_t> after_first_pass_image(image.getShape());
    Matrix<uint8_t>* sampled_image = &image;

    for (int kernel_direction = 0; kernel_direction < 2; kernel_direction++) {
        for (int row = 0; row < image.getShape().y; row++) {
            for (int col = 0; col < image.getShape().x; col++) {
                for (int channel = 0; channel < image.getShape().z; channel++) {

                    float convolution_step_value = 0;
                    for (int kernel_idx = 0; kernel_idx < kernel.getSize(); kernel_idx++) {
                        int image_col_idx = kernel_direction == 0 ? col - (kernel.getSize() - 1) / 2 + kernel_idx : col;
                        int image_row_idx = kernel_direction == 1 ? row - (kernel.getSize() - 1) / 2 + kernel_idx : row;

                        uint8_t image_value = 0;
                        if (image_col_idx >= 0 && image_col_idx < image.getShape().x &&
                            image_row_idx >= 0 && image_row_idx < image.getShape().y) {
                            image_value = sampled_image->at(image_col_idx, image_row_idx, channel);
                        }

                        convolution_step_value += kernel.at1D(kernel_idx) * image_value;
                    }
                    filtered_image.at(col, row, channel) = convolution_step_value;

                    // TODO: magic number
                    if (kernel_direction == 0) {
                        after_first_pass_image.at(col, row, channel) = convolution_step_value;
                    }
                }
            }
        }
        sampled_image = &after_first_pass_image;
    }
}