#include "gtest/gtest.h"
#include "test_utils.h"
#include "../src/grayscale_converter.h"

class GrayscaleConverterTest : public ::testing::Test {
    protected:
        GrayscaleConverterTest()
        { }
};

TEST_F(GrayscaleConverterTest, ShouldConvertColorImageToGrayscale) {
    // given
    Shape image_shape(10, 10, 3);
    Matrix<uint8_t> color_image(image_shape);
    testutils::populateMatrixChannelWithValue(color_image, 0, 10);
    testutils::populateMatrixChannelWithValue(color_image, 1, 20);
    testutils::populateMatrixChannelWithValue(color_image, 2, 30);

    // when
    Matrix<uint8_t> grayscale_image = GrayscaleConverter::convertToGrayscale(color_image);

    // then
    int channels_num = grayscale_image.getShape().z;
    ASSERT_EQ(channels_num, 1);

    uint8_t grayscale_value = static_cast<uint8_t>(10 * 0.299f + 20 * 0.587f + 30 * 0.114f);
    for (size_t col = 0; col < image_shape.x; col++) {
        for (size_t row = 0; row < image_shape.y; row++) {
            ASSERT_EQ(grayscale_image.at(col, row, 0), grayscale_value);
        }
    }
}