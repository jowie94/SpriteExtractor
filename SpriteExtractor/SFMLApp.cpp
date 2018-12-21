#include "SFMLApp.hpp"

#include "imgui-SFML.h"

#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

void SFMLApp::Run()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Sprite Extractor");
    ImGui::SFML::Init(window);

#ifdef __APPLE__
    ImGui::GetIO().FontGlobalScale = 2.0f;
#endif

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

            Loop();

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }
    }
}

template<typename T>
Vec2<T> sfmlVecToVec(const sf::Vector2<T>& rhs)
{
    return Vec2<T>(rhs.x, rhs.y);
}

class SFMLImageResource : public ImageResource
{
public:
    SFMLImageResource(const std::string& filename)
    {
        texture.loadFromFile(filename);
        resourceId = texture.getNativeHandle();
        size = sfmlVecToVec(texture.getSize());
    }

private:
    sf::Texture texture;
};

std::unique_ptr<ImageResource> SFMLApp::OpenImage(const std::string& path)
{
//     sf::Texture sfmlImage;
//     bool result = sfmlImage.loadFromFile(path);

    return std::make_unique<SFMLImageResource>(path);
}
