#include "SFMLApp.hpp"

#include "imgui-sfml/imgui-SFML.h"

#include <ImGui/imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

#include "Logger/Logger.hpp"

#include "Services/Services.hpp"
#include "Services/ImGuiManager/SFMLImGuiManager.hpp"

void SFMLApp::Run()
{
#ifdef __APPLE__
    float scale = 2.0f;
#else
    float scale = 1.0f;
#endif

    sf::ContextSettings settings;
    settings.antialiasingLevel = 5;
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(static_cast<unsigned int>(1366.0f * scale), 
        static_cast<unsigned int>(768.0f * scale)), "Sprite Extractor", sf::Style::Default, settings);

    Services::GetInstance().Get<SFMLImGuiManager>()->SetWindow(window);

    Init();

    ImGui::GetIO().DisplayFramebufferScale = ImVec2(scale, scale);

    Logger::GetLogger("SFML")->info("Beginning App Loop");
    sf::Clock deltaClock;
    while (window->isOpen())
    {
        sf::Event event{};
        while (window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        window->clear();

        Loop();

        window->display();
    }

    Shutdown();
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
    static std::shared_ptr<SFMLImage> Create(const std::string& filename)
    {
        auto image = std::make_shared<SFMLImage>();

        if (image->image.loadFromFile(filename))
        {
            return image;
        }
        return nullptr;
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

std::shared_ptr<IImage> SFMLApp::OpenImage(const std::string& path)
{
    return SFMLImage::Create(path);
}
