#include "ImGuiManager.hpp"

#include "Services/Services.hpp"
#include "Services/Scheduler/Scheduler.hpp"

#include "UI/BaseWindow.hpp"

ImGuiManager::ImGuiManager() = default;
ImGuiManager::~ImGuiManager() = default;

void ImGuiManager::Init()
{
    Services::GetInstance().Get<Scheduler>()->Schedule(this, std::bind(&ImGuiManager::Update, this));
}

void ImGuiManager::Update()
{
    if (_mainWindow)
    {
        _mainWindow->DoDraw();
    }
}

void ImGuiManager::Shutdown()
{
    _mainWindow.reset();
}

void ImGuiManager::SetMainWindow(std::unique_ptr<BaseWindow>&& window)
{
    _mainWindow = std::move(window);
    _mainWindow->Init();
}
