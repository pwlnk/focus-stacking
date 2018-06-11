#pragma once

#include <vector>

class FilterKernel {
    public:
        virtual ~FilterKernel() = 0;

        virtual std::vector<float> getValues() = 0;
        virtual unsigned short getSize() = 0;
        virtual float at(size_t col, size_t row) = 0;
};

inline FilterKernel::~FilterKernel() {}