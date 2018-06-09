#include "gtest/gtest.h"
#include "../src/matrix.h"

class MatrixTest : public ::testing::Test {
    protected:
        Matrix<u_char> matrix;

        static const u_int x_dim = 20;
        static const u_int y_dim = 20;
        static const u_int z_dim = 3;

        MatrixTest() :
        matrix(x_dim, y_dim, z_dim)
        { }
};

TEST_F(MatrixTest, ShouldHave20x20x3Shape) {
    // given
    // when
    Shape matrix_shape = matrix.getShape();

    // then
    ASSERT_EQ(matrix_shape, Shape(x_dim, y_dim, z_dim));
}

TEST_F(MatrixTest, ShouldBeInitializedWithZeros) {
    // given
    // when
    // then
    for (int idx = 0; idx < x_dim * y_dim * z_dim; idx++) {
        ASSERT_EQ(matrix[idx], 0);
    }
}

TEST_F(MatrixTest, ShouldHaveSubscriptOperatorOverloaded) {
    // given
    matrix[10] = 100;

    // when
    // then
    ASSERT_EQ(matrix[10], 100);
}