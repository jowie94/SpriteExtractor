#pragma once

#include <memory>

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
