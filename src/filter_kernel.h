#pragma once

#include <vector>

class FilterKernel {
    public:
        virtual ~FilterKernel() = 0;

        virtual std::vector<float> getValues() = 0;
        virtual unsigned short getSize() = 0;
        virtual float at(int col, int row) = 0;
        virtual float at1D(int idx) = 0;
        virtual bool isSeparableInto1D() = 0;
};

inline FilterKernel::~FilterKernel() {}