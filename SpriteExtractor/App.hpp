#include <string>

template<class T>
struct Vec2
{
    Vec2(T x_, T y_)
    : x(x_)
    , y(y_)
    {
    }

    T x;
    T y;
};

using ImageSize = Vec2<unsigned int>;
struct ImageResource
{
    ImageResource()
    {}

    ImageResource(unsigned int resourceId_, const ImageSize& size_)
    : resourceId(resourceId_)
    , size(size_)
    {}

    bool isValid() const
    {
        return resourceId != 0;
    }

    unsigned int resourceId = 0;
    ImageSize size = ImageSize(0, 0);
};

class App
{
public:
    virtual void Run() = 0;

protected:
    void Loop();

    virtual std::unique_ptr<ImageResource> OpenImage(const std::string& path) = 0;

private:
    std::string selectedFile;
    std::unique_ptr<ImageResource> openedFile;
};