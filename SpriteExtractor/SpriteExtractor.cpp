// SpriteExtractor.cpp : Defines the entry point for the application.
//

#include "SpriteExtractor.h"
#include "CImg.h"

#include <vector>
#include <iostream>

using Image = cimg_library::CImg<unsigned char>;

std::ostream& operator<<(std::ostream& os, const BBox& box)
{
    os << "(X: " << box.X << ", Y: " << box.Y << ", W: " << box.Width << ", H:" << box.Height << ")";

    return os;
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    os << "(" << (int)color.R << "," << (int)color.G << "," << (int)color.B << "," << (int)color.A << ")";

    return os;
}

bool CompareColor(const unsigned char* first, const unsigned char* second)
{
    return first[0] == second[0] && first[1] == second[1] && first[2] == second[2];
}

void PrintColor(unsigned int r, unsigned int g, unsigned int b)
{
    std::cout << "(" << r << "," << g << "," << b << ")" << std::endl;
}

Color ExtractColor(const Image& img, int x, int y)
{
    const unsigned char& r = img(x, y, 0, 0);
    const unsigned char& g = img(x, y, 0, 1);
    const unsigned char& b = img(x, y, 0, 2);

    return Color(r, g, b);
}

bool IsPointInList(const std::vector<BBox>& boxes, int x, int y)
{
    for (const auto& box : boxes)
    {
        if (box.ContainsPoint(x, y))
        {
            return true;
        }
    }

    return false;
}

BBox FindSprite(size_t initialRow, size_t initialColumn, const Matrix<bool>& img)
{
    BBox box;
    box.X = initialColumn;
    box.Y = initialRow;

    size_t minColumn = initialColumn;
    size_t maxColumn = initialColumn;
    size_t maxRow = initialRow;

    const Matrix<bool>::MatrixSize& size = img.Size();
    for (size_t y = initialRow; y < size.second; ++y)
    {
        bool pixelFound = img.At(initialColumn, y);

        if (initialColumn > 0)
        {
            for (size_t x = initialColumn - 1; x >= (minColumn - 1); --x)
            {
                if (img.At(x, y))
                {
                    minColumn = std::min(minColumn, x);
                    pixelFound = true;
                }

                if (x == 0)
                {
                    break;
                }
            }
        }

        for (size_t x = initialColumn; x < size.first && x <= (maxColumn + 1); ++x)
        {
            if (img.At(x, y))
            {
                maxColumn = std::max(maxColumn, x);
                pixelFound = true;
            }
        }

        if (!pixelFound)
        {
            break;
        }

        maxRow = y;
    }

    box.X = minColumn;
    box.Width = maxColumn - box.X;
    box.Height = maxRow - box.Y;

    return box;
}

BBox FindSprite(const int initialRow, const int initialColumn, const Image& img, const Color& filterColor)
{
    BBox box;
    box.X = initialColumn;
    box.Y = initialRow;

    int minColumn = initialColumn;
    int maxColumn = initialColumn;
    int maxRow = initialRow;

    for (int y = initialRow; y < img.height(); ++y)
    {
        bool pixelFound = false;

        for (int x = initialColumn; x >= 0; --x)
        {
            Color pixelColor = ExtractColor(img, x, y);

            if (pixelColor != filterColor)
            {
                minColumn = std::min(minColumn, x);
                pixelFound = true;
            }
            else if (x < minColumn)
            {
                break;
            }
        }

        for (int x = initialColumn; x < img.width(); ++x)
        {
            Color pixelColor = ExtractColor(img, x, y);

            if (pixelColor != filterColor)
            {
                maxColumn = std::max(maxColumn, x);
                pixelFound = true;
            }
            else if (x > maxColumn)
            {
                break;
            }
        }

        if (!pixelFound)
        {
            break;
        }

        maxRow = y;
    }

    box.X = minColumn;
    box.Width = maxColumn - box.X;
    box.Height = maxRow - box.Y;

    return box;
}

void SearchSprites()
{
    Image img("test.png");

    const Color purple(128, 0, 255);

    std::vector<BBox> sprites;

    int found = 0;

    // Find first pixel
    for (int i = 0; i < img.height(); ++i)
    {
        for (int j = 0; j < img.width(); ++j)
        {

            Color color = ExtractColor(img, j, i);

            if (purple != color && !IsPointInList(sprites, j, i))
            {
                std::cout << color << std::endl;
                std::cout << "Sprite found at " << j << " " << i << std::endl;

                BBox sprite = FindSprite(i, j, img, purple);

                std::cout << sprite << std::endl;

                j = sprite.X + sprite.Width; // Skip to the next sprite

                sprites.emplace_back(sprite);

                ++found;
            }
        }

        /*if (found > 30)
        {
            break;
        }*/
    }
    
    const unsigned char red[] = { 255,0,0 };

    for (const auto& sprite : sprites)
    {
        img.draw_rectangle(sprite.X, sprite.Y, sprite.Width + sprite.X, sprite.Y + sprite.Height, red, 1, ~0U);
    }

    img.display();
}

Matrix<bool> SpriteExtractor::GenerateMatrix(const ImageAccessor& callbacks, const Color& filterColor, const void* image)
{
    assert(callbacks.GetWidth);
    assert(callbacks.GetHeight);
    assert(callbacks.GetColor);

    size_t width = callbacks.GetWidth(image);
    size_t height = callbacks.GetHeight(image);

    Matrix<bool> matrix(std::make_pair(width, height));

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            matrix.At(x, y) = callbacks.GetColor(x, y, image) != filterColor;
        }
    }

    return matrix;
}

SpriteExtractor::SpriteList SpriteExtractor::FindSprites(const Matrix<bool>& image)
{
    SpriteList list;

    Matrix<bool>::MatrixSize size = image.Size();
    for (size_t row = 0; row < size.second; ++row)
    {
        for (size_t column = 0; column < size.first; ++column)
        {
            if (image.At(column, row) && !IsPointInList(list, column, row))
            {
                BBox sprite = FindSprite(row, column, image);

                column = sprite.X + sprite.Width; // Skip to the next sprite

                list.emplace_back(sprite);
            }
        }
    }

    return list;
}
