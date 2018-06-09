#include "shape.h"

Shape::Shape(size_t x, size_t y, size_t z) :
x(x), y(y), z(z)
{ }

bool operator==(const Shape& s1, const Shape& s2) {
    return s1.x == s2.x && s1.y == s2.y && s1.z == s2.z;
}