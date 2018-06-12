#pragma once

#include "convolution_strategy.h"

#include <thread>

template <typename TInput, typename TOutput>
class ParallelConvolutionStrategy : public ConvolutionStrategy<TInput, TOutput> {
    private:
        int threads_number;

        Matrix<TInput>* image;
        Matrix<TOutput>* filtered_image;
        FilterKernel* kernel;

        void threadConvolutionChunk2D(int start_row, int end_row);
        void threadConvolutionChunk1D(int start_row, int end_row, int kernel_direction,
                                      Matrix<TOutput>& after_first_pass_image);

        float convolution2DStep(int col, int row, int channel);
        float convolution1DStep(int col, int row, int channel, int kernel_direction,
                                Matrix<TOutput>& after_first_pass_image);

    public:
        ParallelConvolutionStrategy(int threads_number = 4);

        void convolution2D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel);
        void convolution1D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel);
};

template <typename TInput, typename TOutput>
ParallelConvolutionStrategy<TInput, TOutput>::ParallelConvolutionStrategy(int threads_number) :
threads_number(threads_number)
{ }

template <typename TInput, typename TOutput>
void ParallelConvolutionStrategy<TInput, TOutput>::convolution2D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel) {
    this->image = &image;
    this->filtered_image = &filtered_image;
    this->kernel = &kernel;

    std::vector<std::thread*> threads;

    for (int thread_idx = 0; thread_idx < threads_number; thread_idx++) {
        int rows_per_thread = static_cast<int>(image.getShape().y / threads_number);
        int start_row = thread_idx * rows_per_thread;
        int end_row = thread_idx != threads_number - 1 ? start_row + rows_per_thread : image.getShape().y;

        threads.push_back(new std::thread([this, start_row, end_row]
                                          { this->threadConvolutionChunk2D(start_row, end_row); }));
    }


    for (auto convolution_thread : threads) {
        convolution_thread->join();
        delete convolution_thread;
    }
}

template <typename TInput, typename TOutput>
void ParallelConvolutionStrategy<TInput, TOutput>::threadConvolutionChunk2D(int start_row, int end_row) {
    for (int row = start_row; row < end_row; row++) {
        for (int col = 0; col < image->getShape().x; col++) {
            for (int channel = 0; channel < image->getShape().z; channel++) {
                filtered_image->at(col, row, channel) = convolution2DStep(col, row, channel);
            }
        }
    }
}

template <typename TInput, typename TOutput>
float ParallelConvolutionStrategy<TInput, TOutput>::convolution2DStep(int col, int row, int channel) {
    float convolution_step_value = 0.0f;
    for (int kernel_col = 0; kernel_col < kernel->getSize(); kernel_col++) {
        for (int kernel_row = 0; kernel_row < kernel->getSize(); kernel_row++) {
            int image_col_idx = col - (kernel->getSize() - 1) / 2 + kernel_col;
            int image_row_idx = row - (kernel->getSize() - 1) / 2 + kernel_row;

            uint8_t image_value = 0;
            if (image_col_idx >= 0 && image_col_idx < image->getShape().x &&
                image_row_idx >= 0 && image_row_idx < image->getShape().y) {
                image_value = image->at(image_col_idx, image_row_idx, channel);
            }

            convolution_step_value += kernel->at(kernel_col, kernel_row) * image_value;
        }
    }

    return convolution_step_value;
}

template <typename TInput, typename TOutput>
void ParallelConvolutionStrategy<TInput, TOutput>::convolution1D(Matrix<TInput>& image, Matrix<TOutput>& filtered_image, FilterKernel& kernel) {
    this->image = &image;
    this->filtered_image = &filtered_image;
    this->kernel = &kernel;

    Matrix<TOutput> after_first_pass_image(image.getShape());

    for (int kernel_direction = 0; kernel_direction < 2; kernel_direction++) {
        std::vector<std::thread*> threads;

        for (int thread_idx = 0; thread_idx < threads_number; thread_idx++) {
            int rows_per_thread = static_cast<int>(image.getShape().y / threads_number);
            int start_row = thread_idx * rows_per_thread;
            int end_row = thread_idx != threads_number - 1 ? start_row + rows_per_thread : image.getShape().y;

            threads.push_back(new std::thread([this, start_row, end_row, kernel_direction, &after_first_pass_image]
                                              { this->threadConvolutionChunk1D(start_row, end_row, kernel_direction, after_first_pass_image); }));
        }


        for (auto convolution_thread : threads) {
            convolution_thread->join();
            delete convolution_thread;
        }
    }
}

template <typename TInput, typename TOutput>
void ParallelConvolutionStrategy<TInput, TOutput>::threadConvolutionChunk1D(int start_row, int end_row, int kernel_direction, Matrix<TOutput>& after_first_pass_image) {
    for (int row = start_row; row < end_row; row++) {
        for (int col = 0; col < image->getShape().x; col++) {
            for (int channel = 0; channel < image->getShape().z; channel++) {
                float convolution_step_value = convolution1DStep(col, row, channel, kernel_direction, after_first_pass_image);
                filtered_image->at(col, row, channel) = convolution_step_value;

                // TODO: get rid of magic number
                if (kernel_direction == 0) {
                    after_first_pass_image.at(col, row, channel) = convolution_step_value;
                }
            }
        }
    }
}

template <typename TInput, typename TOutput>
float ParallelConvolutionStrategy<TInput, TOutput>::convolution1DStep(int col, int row, int channel, int kernel_direction,
                                                                      Matrix<TOutput>& after_first_pass_image) {
    float convolution_step_value = 0;
    for (int kernel_idx = 0; kernel_idx < kernel->getSize(); kernel_idx++) {
        int image_col_idx = kernel_direction == 0 ? col - (kernel->getSize() - 1) / 2 + kernel_idx : col;
        int image_row_idx = kernel_direction == 1 ? row - (kernel->getSize() - 1) / 2 + kernel_idx : row;

        float image_value = 0.0;
        if (image_col_idx >= 0 && image_col_idx < image->getShape().x &&
            image_row_idx >= 0 && image_row_idx < image->getShape().y) {
            image_value = kernel_direction == 0 ?
                          image->at(image_col_idx, image_row_idx, channel) :
                          after_first_pass_image.at(image_col_idx, image_row_idx, channel);
        }

        convolution_step_value += kernel->at1D(kernel_idx) * image_value;
    }

    return convolution_step_value;
}