#include "imgui-SFML.h"
#include "Platform/GenericPlatform.h"

#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <string>

int main()
{
    static std::vector<Platform::FileFilter> imgFilter =
    {
        { "Images", "*.png;*.jpg" },
        { "All", "*.*" }
    };

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Sprite Extractor");
    ImGui::SFML::Init(window);

    std::string selectedFile = "";

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

            //ImGui::ShowTestWindow();

            ImGui::Begin("");

            ImGui::Text("File: %s", selectedFile.c_str());

            const bool button = ImGui::Button("Open File");

            if (button)
            {
                std::string file;
                if (Platform::ShowOpenFileDialogue("Choose an sprite sheet image", file, imgFilter))
                {
                    selectedFile = file;
                }
            }

            ImGui::End();


            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }
    }

    return 0;
}
