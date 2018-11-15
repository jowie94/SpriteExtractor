// SpriteExtractor.cpp : Defines the entry point for the application.
//

#include "SpriteExtractor.h"
#include "CImg.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <imgui.h>

#include <shlwapi.h>

HRESULT BasicFileOpen()
{
    // CoCreate the File Open Dialog object.
    IFileOpenDialog *pfd = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pfd));

    COMDLG_FILTERSPEC img[] =
    {
        { L"Images", L"*.png;*.jpg" },
        { L"All", L"*.*" }
    };
         
    if (SUCCEEDED(hr))
    {
        // Create an event handling object, and hook it up to the dialog.
        //IFileDialogEvents *pfde = NULL;
        //hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
        if (SUCCEEDED(hr))
        {
            // Hook up the event handler.
            DWORD dwCookie;
            //hr = pfd->Advise(pfde, &dwCookie);
            if (SUCCEEDED(hr))
            {
                // Set the options on the dialog.
                DWORD dwFlags;

                // Before setting, always get the options first in order 
                // not to override existing options.
                hr = pfd->GetOptions(&dwFlags);
                if (SUCCEEDED(hr))
                {
                    // In this case, get shell items only for file system items.
                    hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                    if (SUCCEEDED(hr))
                    {
                        // Set the file types to display only. 
                        // Notice that this is a 1-based array.
                        hr = pfd->SetFileTypes(ARRAYSIZE(img), img);
                        if (SUCCEEDED(hr))
                        {
                            // Set the selected file type index to Word Docs for this example.
                            //hr = pfd->SetFileTypeIndex(INDEX_IMAGES);
                            if (SUCCEEDED(hr))
                            {
                                // Set the default extension to be ".doc" file.
                                //hr = pfd->SetDefaultExtension(L"doc;docx");
                                if (SUCCEEDED(hr))
                                {
                                    // Show the dialog
                                    hr = pfd->Show(NULL);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Obtain the result once the user clicks 
                                        // the 'Open' button.
                                        // The result is an IShellItem object.
                                        IShellItem *psiResult;
                                        hr = pfd->GetResult(&psiResult);
                                        if (SUCCEEDED(hr))
                                        {
                                            // We are just going to print out the 
                                            // name of the file for sample sake.
                                            PWSTR pszFilePath = NULL;
                                            hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                                &pszFilePath);
                                            if (SUCCEEDED(hr))
                                            {
                                                CoTaskMemFree(pszFilePath);
                                            }
                                            psiResult->Release();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // Unhook the event handler.
                //pfd->Unadvise(dwCookie);
            }
            //pfde->Release();
        }
        pfd->Release();
    }
    return hr;
}

using Image = cimg_library::CImg<unsigned char>;

template<typename T = unsigned char>
struct Color
{
    Color(T R_ = 0, T G_ = 0, T B_ = 0, T A_ = 0)
    : R(R_)
    , G(G_)
    , B(B_)
    , A(A_)
    {}

    T R = 0;
    T G = 0;
    T B = 0;
    T A = 0;

    bool operator==(const Color& other) const
    {
        return R == other.R && G == other.G && B == other.B && A == other.A;
    }

    bool operator!=(const Color& other) const
    {
        return !operator==(other);
    }
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

std::ostream& operator<<(std::ostream& os, const BBox& box)
{
    os << "(X: " << box.X << ", Y: " << box.Y << ", W: " << box.Width << ", H:" << box.Height << ")";

    return os;
}

std::ostream& operator<<(std::ostream& os, const Color<>& color)
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

Color<> ExtractColor(const Image& img, int x, int y)
{
    const unsigned char& r = img(x, y, 0, 0);
    const unsigned char& g = img(x, y, 0, 1);
    const unsigned char& b = img(x, y, 0, 2);

    return Color<>(r, g, b);
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

BBox FindSprite(const int initialRow, const int initialColumn, const Image& img, const Color<>& filterColor)
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
            Color<> pixelColor = ExtractColor(img, x, y);

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
            Color<> pixelColor = ExtractColor(img, x, y);

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

    const Color<> purple(128, 0, 255);

    std::vector<BBox> sprites;

    int found = 0;

    // Find first pixel
    for (int i = 0; i < img.height(); ++i)
    {
        for (int j = 0; j < img.width(); ++j)
        {

            Color<> color = ExtractColor(img, j, i);

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

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "Sprite Extractor");
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            ImGui::SFML::Update(window, deltaClock.restart());

            ImGui::ShowTestWindow();

            ImGui::Begin("Hello, world!");

            const bool button = ImGui::Button("Look at this pretty button");

            if (button)
            {
                BasicFileOpen();
            }

            ImGui::End();


            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }
    }

    return 0;
}
