#include "focus_stack.h"
#include "gaussian_kernel.h"
#include "laplacian_kernel.h"
#include "grayscale_converter.h"
#include "image_filter.h"

FocusStack::FocusStack() :
all_in_focus_image(1, 1, 1), depth_map(1, 1, 1)
{ }

void FocusStack::addImage(Matrix<uint8_t> image) {
    images_stack.push_back(image);
}

void FocusStack::computeAllInFocusAndDepthMap(const int bg_threshold) {
    Shape images_shape = images_stack[0].getShape();
    depth_map = Matrix<uint8_t>(images_shape.x, images_shape.y);
    all_in_focus_image = Matrix<uint8_t>(images_shape);

    std::vector<Matrix<float>> log_stack;
    computeLaplacianOfGaussianStack(log_stack);
    computeAllInFocusAndDepthMap(log_stack, bg_threshold);
}

void FocusStack::computeLaplacianOfGaussianStack(std::vector<Matrix<float>>& log_stack) {
    GaussianKernel gaussian_kernel;
    LaplacianKernel laplacian_kernel;
    ImageFilter<uint8_t, uint8_t> gaussian_filter(gaussian_kernel);
    ImageFilter<uint8_t, float> laplacian_filter(laplacian_kernel);

    int image_number = 1;
    for (auto image : images_stack) {
        // TODO: remove this cout
        std::cout << "processing image " << image_number++ << "/" << images_stack.size() << std::endl;

        Matrix<uint8_t> grayscale_image = GrayscaleConverter::convertToGrayscale(image);
        Matrix<uint8_t> blurred_image = gaussian_filter.convolution(grayscale_image);
        Matrix<float> log_image = laplacian_filter.convolution(blurred_image);
        log_stack.push_back(log_image);
    }
}

void FocusStack::computeAllInFocusAndDepthMap(std::vector<Matrix<float>>& log_stack, const int bg_threshold) {
    Shape images_shape = images_stack[0].getShape();
    for (int row = 0; row < images_shape.y; row++) {
        for (int col = 0; col < images_shape.x; col++) {
            int max_laplacian_idx = maxLoGIdx(col, row, log_stack, bg_threshold);

            int stack_idx = max_laplacian_idx == log_stack.size() ? log_stack.size() - 1 : max_laplacian_idx;
            all_in_focus_image.at(col, row, 0) = images_stack.at(stack_idx).at(col, row, 0);
            all_in_focus_image.at(col, row, 1) = images_stack.at(stack_idx).at(col, row, 1);
            all_in_focus_image.at(col, row, 2) = images_stack.at(stack_idx).at(col, row, 2);

            depth_map.at(col, row, 0) = 255 - max_laplacian_idx * (255 / (log_stack.size() + 1));
        }
    }
}

int FocusStack::maxLoGIdx(int col, int row, std::vector<Matrix<float>> &log_stack,
                          const int bg_threshold) {
    int max_laplacian_idx = log_stack.size();
    float max_laplacian_value = 0;

    for (int stack_idx = 0; stack_idx < log_stack.size(); stack_idx++) {
        if (abs(log_stack.at(stack_idx).at(col, row, 0)) > max_laplacian_value &&
            abs(log_stack.at(stack_idx).at(col, row, 0)) > bg_threshold)
        {
            max_laplacian_idx = stack_idx;
            max_laplacian_value = abs(log_stack.at(stack_idx).at(col, row, 0));
        }
    }

    return max_laplacian_idx;
};

Matrix<uint8_t> FocusStack::getAllInFocusImage() const {
    return all_in_focus_image;
}

Matrix<uint8_t> FocusStack::getDepthMap() const {
    return depth_map;
}