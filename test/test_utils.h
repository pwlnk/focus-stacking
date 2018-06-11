#pragma once

#include <stdint.h>
#include "../src/matrix.h"

namespace testutils {

    void populateMatrixChannelWithValue(Matrix<uint8_t>& matrix, size_t channel, uint8_t value);

}