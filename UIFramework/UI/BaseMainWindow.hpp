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
    virtual ~BaseMainWindow() = default;

protected:
	template<typename T, typename... Args>
	std::weak_ptr<T> AddPanel(Args&&... args)
	{
		static_assert(std::is_base_of<PanelWindow, T>::value, "Can't convert T to IPanelWidget");
		std::shared_ptr<T> panel = std::make_shared<T>(std::forward<Args...>(args...));
		AddPanelInt(panel);
		return panel;
	}

	PopupsController& GetPopupsController();

private:
	void AddPanelInt(std::shared_ptr<PanelWindow> panel);

	void BeginWidget() override final;
	void EndWidget() override final;

	void SetupLayout();

	std::vector<std::shared_ptr<PanelWindow>> _panels;
	PopupsController _popupsController;
};
