#pragma once

#include <cstddef>

struct Shape {
    size_t x, y, z;

    Shape(size_t x = 1, size_t y = 1, size_t z = 1);
};

bool operator==(const Shape& s1, const Shape& s2);