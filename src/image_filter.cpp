#include <iostream>
#include "image_filter.h"

ImageFilter::ImageFilter()
{ }

short ImageFilter::getSamePaddingValue(short kernel_size) {
    return (kernel_size - 1) / 2;
}

// TODO: result matrix cannot be uint (negative values possible)
Matrix<uint8_t> ImageFilter::convolution(Matrix<uint8_t>& image, FilterKernel& kernel) {
    Shape result_image_shape = image.getShape();
    Matrix<uint8_t> filtered_image(result_image_shape);

    short padding = getSamePaddingValue(kernel.getSize());
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

    return filtered_image;
}