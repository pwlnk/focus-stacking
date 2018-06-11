#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_utils.h"
#include "../src/image_filter.h"

using ::testing::Return;
using ::testing::_;

class MockFilterKernel : public FilterKernel {
    public:
        MOCK_METHOD0(getValues, std::vector<float>());
        MOCK_METHOD0(getSize, unsigned short());
        MOCK_METHOD1(at1D, float(int));
        MOCK_METHOD2(at, float(int, int));
        MOCK_METHOD0(isSeparableInto1D, bool());
};

class ImageFilterTest : public ::testing::Test {
    protected:
        Matrix<uint8_t> image;
        Matrix<uint8_t> expected_image;
        
        void populateExpectedImage();
    
        ImageFilterTest() :
        image(4, 4, 3), expected_image(4, 4, 3)
        {
            testutils::populateMatrixChannelWithValue(image, 0, 2);
            testutils::populateMatrixChannelWithValue(image, 1, 2);
            testutils::populateMatrixChannelWithValue(image, 2, 2);
            populateExpectedImage();
        }
};

TEST_F(ImageFilterTest, ShouldPerform1DConvolutionIfPossible) {
    // given
    MockFilterKernel mock_kernel;
    ImageFilter image_filter(mock_kernel);    

    EXPECT_CALL(mock_kernel, getSize())
            .WillRepeatedly(Return(3));
    EXPECT_CALL(mock_kernel, isSeparableInto1D())
            .WillOnce(Return(true));
    EXPECT_CALL(mock_kernel, at1D)
            .WillRepeatedly(Return(1));

    // when
    Matrix<uint8_t> filtered_image = image_filter.convolution(image);
    Shape image_shape = image.getShape();
    
    // then
    ASSERT_EQ(filtered_image.getShape(), image_shape);
    for (int row = 0; row < image_shape.y; row++) {
        for (int col = 0; col < image_shape.x; col++) {
            for (int channel = 0; channel < image_shape.z; channel++) {
                ASSERT_EQ(filtered_image.at(col, row, channel), expected_image.at(col, row, channel));
            }
        }
    }
}

TEST_F(ImageFilterTest, ShouldPerform2DConvolutionIf1DImpossible) {
    // given
    MockFilterKernel mock_kernel;
    ImageFilter image_filter(mock_kernel);

    EXPECT_CALL(mock_kernel, getSize())
            .WillRepeatedly(Return(3));
    EXPECT_CALL(mock_kernel, isSeparableInto1D())
            .WillOnce(Return(false));
    EXPECT_CALL(mock_kernel, at)
            .WillRepeatedly(Return(1));

    // when
    Matrix<uint8_t> filtered_image = image_filter.convolution(image);
    Shape image_shape = image.getShape();

    // then
    ASSERT_EQ(filtered_image.getShape(), image_shape);
    for (int row = 0; row < image_shape.y; row++) {
        for (int col = 0; col < image_shape.x; col++) {
            for (int channel = 0; channel < image_shape.z; channel++) {
                ASSERT_EQ(filtered_image.at(col, row, channel), expected_image.at(col, row, channel));
            }
        }
    }
}

void ImageFilterTest::populateExpectedImage() {
    expected_image[0] = expected_image[16] = expected_image[32] = 8;
    expected_image[1] = expected_image[17] = expected_image[33] = 12;
    expected_image[2] = expected_image[18] = expected_image[34] = 12;
    expected_image[3] = expected_image[19] = expected_image[35] = 8;
    expected_image[4] = expected_image[20] = expected_image[36] = 12;
    expected_image[5] = expected_image[21] = expected_image[37] = 18;
    expected_image[6] = expected_image[22] = expected_image[38] = 18;
    expected_image[7] = expected_image[23] = expected_image[39] = 12;
    expected_image[8] = expected_image[24] = expected_image[40] = 12;
    expected_image[9] = expected_image[25] = expected_image[41] = 18;
    expected_image[10] = expected_image[26] = expected_image[42] = 18;
    expected_image[11] = expected_image[27] = expected_image[43] = 12;
    expected_image[12] = expected_image[28] = expected_image[44] = 8;
    expected_image[13] = expected_image[29] = expected_image[45] = 12;
    expected_image[14] = expected_image[30] = expected_image[46] = 12;
    expected_image[15] = expected_image[31] = expected_image[47] = 8;
}