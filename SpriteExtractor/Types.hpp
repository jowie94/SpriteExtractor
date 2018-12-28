#pragma once

#include <memory>
#include <utility>
#include <cassert>

template<class T>
struct Vec2
{
    Vec2()
    {}

    Vec2(T x_, T y_)
        : x(x_)
        , y(y_)
    {
    }

    T x = 0;
    T y = 0;
};

struct Color
{
    using channel_t = unsigned char;
    Color()
    {}

    Color(channel_t r_, channel_t g_, channel_t b_, channel_t a_ = 255)
        : r(r_)
        , g(g_)
        , b(b_)
        , a(a_)
    {}

    channel_t r = 0;
    channel_t g = 0;
    channel_t b = 0;
    channel_t a = 0;
};

using ImageSize = Vec2<unsigned int>;
struct ITextureResource
{
    unsigned int resourceId = 0;
    ImageSize size;
};

using ImageSize = Vec2<unsigned int>;
class IImage
{
public:
    virtual ~IImage() = default;
    virtual ImageSize Size() const = 0;
    virtual std::unique_ptr<ITextureResource> GetTextureResource() const = 0;

    virtual Color GetPixel(unsigned int x, unsigned int y) const = 0;
};

template<typename T>
class Matrix
{
public:
    using MatrixSize = std::pair<size_t, size_t>;

    Matrix(const MatrixSize& size_)
        : size(size_)
        , data(new T[](size.first * size.second))
    {
        assert(size.first != 0 || size.second != 0 && "Invalid size");
    }

    Matrix(const MatrixSize& size_, const T& initialValue)
        : size(size_)
        , data(new T[](size.first * size.second))
    {
        assert(size.first != 0 || size.second != 0 && "Invalid size");

        for (size_t i = 0; i < size.first * size.second; ++i)
        {
            data[i] = initialValue;
        }
    }

    Matrix(const Matrix& other)
        : size(other.size)
        , data(new T[](size.first * size.second))
    {
        memcpy(data, other.data, size.first * size.second);
    }

    Matrix(Matrix&& other) noexcept
        : size(other.size)
        , data(other.data)
    {
        other.size = std::make_pair(0, 0);
        other.data = nullptr;
    }

    ~Matrix()
    {
        delete[] data;
    }

    const T& At(size_t i, size_t j) const
    {
        assert(0 <= i && i < size.first && "Invalid i coordinate (column)");
        assert(0 <= j && j < size.second && "Invalid j coordinate (row)");
        return data[i * size.second + j];
    }

    T& At(size_t i, size_t j)
    {
        assert(0 <= i && i < size.first && "Invalid i coordinate (column)");
        assert(0 <= j && j < size.second && "Invalid j coordinate (row)");
        return data[i * size.second + j];
    }

    const MatrixSize& Size() const
    {
        return size;
    }

private:
    MatrixSize size;
    T* data;
};
