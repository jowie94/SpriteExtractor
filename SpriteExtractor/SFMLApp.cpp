#include "SFMLApp.hpp"

#include "imgui-SFML.h"

#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

void SFMLApp::Run()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Sprite Extractor");
    ImGui::SFML::Init(window);

#ifdef __APPLE__
    ImGui::GetIO().FontGlobalScale = 2.0f;
#endif

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
        texture.loadFromImage(image);
        resourceId = texture.getNativeHandle();
        size = sfmlVecToVec(texture.getSize());
    }

    SFMLTextureResource(const std::string& filename)
    {
        texture.loadFromFile(filename);
        resourceId = texture.getNativeHandle();
        size = sfmlVecToVec(texture.getSize());
    }

private:
    sf::Texture texture;
};

class SFMLImage : public IImage
{
public:
    SFMLImage(const std::string& filename)
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

private:
    sf::Image image;
};

std::unique_ptr<IImage> SFMLApp::OpenImage(const std::string& path)
{
    return std::make_unique<SFMLImage>(path);
}
