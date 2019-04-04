#pragma once

#include <vector>
#include <memory>

#include "BaseWindow.hpp"
#include "PanelWindow.hpp"
#include "Controllers/PopupsController.hpp"

class BaseMainWindow : public BaseWindow
{
public:
    explicit BaseMainWindow(const char* name);
    virtual ~BaseMainWindow();

protected:
    void Draw() override;

    template<typename T, typename... Args>
    std::weak_ptr<T> AddPanel(PanelWindow::Position initialPosition, Args&&... args)
    {
        static_assert(std::is_base_of<PanelWindow, T>::value, "Can't convert T to IPanelWidget");
        std::shared_ptr<T> panel = std::make_shared<T>(std::forward<Args>(args)...);
        AddPanelInt(initialPosition, panel);
        return panel;
    }

    PopupsController& GetPopupsController();

private:
    void AddPanelInt(PanelWindow::Position initialPosition, std::shared_ptr<PanelWindow> panel);

    bool BeginWidget() override final;
    void EndWidget(bool wasDrawn) override final;

    void SetupLayout();

    std::vector<struct PanelContainer> _panels;
    PopupsController _popupsController;
};
