#include "SFMLImGuiManager.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Services/Services.hpp"
#include "imgui-sfml/imgui-SFML.h"

void SFMLImGuiManager::SetWindow(std::shared_ptr<sf::RenderWindow> window)
{
    ImGuiManager::Init();
    _window = std::move(window);
}

void SFMLImGuiManager::Init()
{
    _deltaClock = std::make_unique<sf::Clock>();
    ImGui::SFML::Init(*_window);
}

void SFMLImGuiManager::Update()
{
    ImGui::SFML::Update(*_window, _deltaClock->restart());

    ImGuiManager::Update();

    ImGui::SFML::Render(*_window);
}

REGISTER_SERVICE_INTERFACE(SFMLImGuiManager, ImGuiManager);
