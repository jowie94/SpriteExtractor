#pragma once

#include <memory>

class BaseWindow;

class ImGuiManager
{
public:
    ImGuiManager();
    virtual ~ImGuiManager();

    void Init();
    virtual void Update();
    void Shutdown();

    template<typename Window, typename... Args>
    void SetMainWindow(Args&&... args)
    {
        static_assert(std::is_base_of_v<BaseWindow, Window>);
        _mainWindow = std::make_unique<Window>(std::forward(args)...);
        SetMainWindow(std::move(_mainWindow));
    }

private:
    void SetMainWindow(std::unique_ptr<BaseWindow>&& window);

    std::unique_ptr<BaseWindow> _mainWindow;
};
