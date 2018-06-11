#include "grayscale_converter.h"

GrayscaleConverter::GrayscaleConverter()
{ }

Matrix<uint8_t> GrayscaleConverter::convertToGrayscale(const Matrix<uint8_t>& color_image) {
    Matrix<uint8_t> grayscale_image(color_image.getShape().x, color_image.getShape().y);

    for (size_t col = 0; col < color_image.getShape().x; col++) {
        for (size_t row = 0; row < color_image.getShape().y; row++) {
            // TODO: do something with magic numbers
            // formula based on https://docs.opencv.org/3.1.0/de/d25/imgproc_color_conversions.html
            float grayscale_value = color_image.at(col, row, 0) * 0.299f +
                                    color_image.at(col, row, 1) * 0.587f +
                                    color_image.at(col, row, 2) * 0.114f;
            grayscale_image.at(col, row, 0) = static_cast<uint8_t>(grayscale_value);
        }
    }

    return grayscale_image;
}