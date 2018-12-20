#include "SFMLApp.hpp"

#include "imgui-SFML.h"

#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

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
