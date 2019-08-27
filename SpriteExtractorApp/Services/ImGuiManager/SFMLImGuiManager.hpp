#pragma once

#include "ImGuiManager.hpp"

namespace sf
{
    class RenderWindow;
    class Clock;
}

class SFMLImGuiManager : public ImGuiManager
{
public:
    void SetWindow(std::shared_ptr<sf::RenderWindow> window);

    void Init() override;
    void Update() override;

private:
    std::shared_ptr<sf::RenderWindow> _window;
    std::unique_ptr<sf::Clock> _deltaClock;
};
