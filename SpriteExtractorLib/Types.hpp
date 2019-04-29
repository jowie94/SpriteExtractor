#pragma once

#include <memory>
#include <utility>
#include <cassert>
#include <vector>

template<class T>
struct Vec2
{
    Vec2()
    {}

    Vec2(T x_, T y_)
        : X(x_)
        , Y(y_)
    {
    }

    T X = 0;
    T Y = 0;
};

struct Color final
{
    using channel_t = unsigned char;
    Color()
    {}

    Color(channel_t r, channel_t g, channel_t b, channel_t a = 255)
        : R(r)
        , G(g)
        , B(b)
        , A(a)
    {}

    void ToFloat(float colors[4]) const
    {
        static float sc = 1.0f / 255.0f;
        colors[0] = R * sc;
        colors[1] = G * sc;
        colors[2] = B * sc;
        colors[3] = A * sc;
    }

    Color& operator=(const float colors[4])
    {
        R = static_cast<channel_t>(colors[0] * 255.0f);
        G = static_cast<channel_t>(colors[1] * 255.0f);
        B = static_cast<channel_t>(colors[2] * 255.0f);
        A = static_cast<channel_t>(colors[3] * 255.0f);

        return *this;
    }

    bool operator==(const Color& other) const
    {
        return R == other.R && G == other.G && B == other.B && A == other.A;
    }

    bool operator!=(const Color& other) const
    {
        return !operator==(other);
    }

    channel_t R = 0;
    channel_t G = 0;
    channel_t B = 0;
    channel_t A = 255;
};

using ImageSize = Vec2<unsigned int>;
struct ITextureResource
{
    unsigned int ResourceId = 0;
    ImageSize Size;
};

using ImageSize = Vec2<unsigned int>;
class IImage
{
public:
    virtual ~IImage() = default;
    virtual ImageSize Size() const = 0;
    virtual std::unique_ptr<ITextureResource> GetTextureResource() const = 0;
    virtual bool Save(const char* filename) const = 0;

    virtual Color GetPixel(size_t x, size_t y) const = 0;
};

template<typename T>
class Matrix
{
public:
    using MatrixSize = std::pair<size_t, size_t>;

    Matrix(const MatrixSize& size_)
        : _size(size_)
        , _data(new T[_size.first * _size.second])
    {
        assert(_size.first != 0 || _size.second != 0 && "Invalid size");
    }

    Matrix(const MatrixSize& size_, const T& initialValue)
        : _size(size_)
        , _data(new T[_size.first * _size.second])
    {
        assert(_size.first != 0 || _size.second != 0 && "Invalid size");

        for (size_t i = 0; i < _size.first * _size.second; ++i)
        {
            _data[i] = initialValue;
        }
    }

    Matrix(const Matrix& other)
        : _size(other._size)
        , _data(new T[_size.first * _size.second])
    {
        memcpy(_data, other._data, _size.first * _size.second);
    }

    Matrix(Matrix&& other) noexcept
        : _size(other._size)
        , _data(other._data)
    {
        other._size = std::make_pair(0, 0);
        other._data = nullptr;
    }

    ~Matrix()
    {
        delete[] _data;
    }

    const T& At(size_t i, size_t j) const
    {
        assert(0 <= i && i < _size.first && "Invalid i coordinate (column)");
        assert(0 <= j && j < _size.second && "Invalid j coordinate (row)");
        return _data[i * _size.second + j];
    }

    T& At(size_t i, size_t j)
    {
        assert(0 <= i && i < _size.first && "Invalid i coordinate (column)");
        assert(0 <= j && j < _size.second && "Invalid j coordinate (row)");
        return _data[i * _size.second + j];
    }

    const MatrixSize& Size() const
    {
        return _size;
    }

private:
    MatrixSize _size;
    T* _data;
};

struct BBox
{
    int X = 0;
    int Y = 0;
    int Width = 0;
    int Height = 0;

    bool ContainsPoint(int x, int y) const
    {
        int mX = X + Width;
        int mY = Y + Height;
        return X <= x && x <= mX && Y <= y && y <= mY;
    }
};
