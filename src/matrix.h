#pragma once

#include <vector>
#include "shape.h"

template <typename T>
class Matrix {
    private:
        Shape shape;
        std::vector<T> data;

    public:
        Matrix(Shape shape);
        Matrix(size_t x, size_t y, size_t z = 1);

        Shape getShape() const;
        T& at(size_t x, size_t y, size_t z);
        const T& at(size_t x, size_t y, size_t z) const;

        T& operator[](const int index);
        const T& operator[](const int index) const;
};

template <typename T>
Matrix<T>::Matrix(Shape shape) :
shape(shape)
{
    data.resize(shape.x * shape.y * shape.z, 0);
}

template <typename T>
Matrix<T>::Matrix(size_t x, size_t y, size_t z) :
Matrix(Shape(x, y, z))
{ }

template <typename T>
Shape Matrix<T>::getShape() const {
    return shape;
}
template <typename T>
T& Matrix<T>::operator[](const int index) {
    return data.at(index);
}

template <typename T>
const T& Matrix<T>::operator[](const int index) const {
    return data.at(index);
}

template <typename T>
T& Matrix<T>::at(size_t x, size_t y, size_t z) {
    return data.at(shape.x * shape.y * z + shape.x * y + x);
}

template <typename T>
const T& Matrix<T>::at(size_t x, size_t y, size_t z) const {
    return data.at(shape.x * shape.y * z + shape.x * y + x);
}