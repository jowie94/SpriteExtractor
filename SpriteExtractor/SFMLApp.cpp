#include "SFMLApp.hpp"

#include "imgui-sfml/imgui-SFML.h"

#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

void SFMLApp::Run()
{
#ifdef __APPLE__
    float scale = 2.0f;
#else
    float scale = 1.0f;
#endif

    sf::RenderWindow window(sf::VideoMode(1366 * scale, 768 * scale), "Sprite Extractor");

    ImGui::SFML::Init(window);

    ImGui::GetIO().DisplayFramebufferScale = ImVec2(scale, scale);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        Loop();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
}

template<typename T>
Vec2<T> sfmlVecToVec(const sf::Vector2<T>& rhs)
{
    return Vec2<T>(rhs.x, rhs.y);
}

Color sfmlColorToColor(const sf::Color& color)
{
    return Color(color.r, color.g, color.b, color.a);
}

class SFMLTextureResource : public ITextureResource
{
public:
    SFMLTextureResource(const sf::Image& image)
    {
        _texture.loadFromImage(image);
        ResourceId = _texture.getNativeHandle();
        Size = sfmlVecToVec(_texture.getSize());
    }

    SFMLTextureResource(const std::string& filename)
    {
        _texture.loadFromFile(filename);
        ResourceId = _texture.getNativeHandle();
        Size = sfmlVecToVec(_texture.getSize());
    }

private:
    sf::Texture _texture;
};

class SFMLImage : public IImage
{
public:
    explicit SFMLImage(const std::string& filename)
    {
        image.loadFromFile(filename);
    }

    ImageSize Size() const override
    {
        return sfmlVecToVec(image.getSize());
    }

    std::unique_ptr<ITextureResource> GetTextureResource() const override
    {
        return std::make_unique<SFMLTextureResource>(image);
    }

    Color GetPixel(size_t x, size_t y) const override
    {
        return sfmlColorToColor(image.getPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
    }

    bool Save(const char* filename) const override
    {
        return image.saveToFile(filename);
    }

private:
    sf::Image image;
};

std::unique_ptr<IImage> SFMLApp::OpenImage(const std::string& path)
{
    return std::make_unique<SFMLImage>(path);
}
