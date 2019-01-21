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

    void ToFloat(float colors[4])
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
    virtual bool Save(const char* filename) const = 0;

    virtual Color GetPixel(size_t x, size_t y) const = 0;
};

template<typename T>
class Matrix
{
public:
    using MatrixSize = std::pair<size_t, size_t>;

    Matrix(const MatrixSize& size_)
        : size(size_)
        , data(new T[size.first * size.second])
    {
        assert(size.first != 0 || size.second != 0 && "Invalid size");
    }

    Matrix(const MatrixSize& size_, const T& initialValue)
        : size(size_)
        , data(new T[size.first * size.second])
    {
        assert(size.first != 0 || size.second != 0 && "Invalid size");

        for (size_t i = 0; i < size.first * size.second; ++i)
        {
            data[i] = initialValue;
        }
    }

    Matrix(const Matrix& other)
        : size(other.size)
        , data(new T[size.first * size.second])
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
