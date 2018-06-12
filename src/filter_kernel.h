#pragma once

#include <vector>

class FilterKernel {
    protected:
        unsigned short ksize;
        std::vector<float> kernel_values;

    public:
        virtual ~FilterKernel();

        virtual std::vector<float> getValues();
        virtual unsigned short getSize();
        virtual float at(int col, int row);
        virtual float at1D(int idx);
        virtual bool isSeparableInto1D();
};