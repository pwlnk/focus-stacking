#include "test_utils.h"

namespace testutils {

    void populateMatrixChannelWithValue(Matrix<uint8_t>& matrix, size_t channel, uint8_t value) {
        Shape matrix_shape = matrix.getShape();

        for (size_t col = 0; col < matrix_shape.x; col++) {
            for (size_t row = 0; row < matrix_shape.y; row++) {
                matrix.at(col, row, channel) = value;
            }
        }
    }

}