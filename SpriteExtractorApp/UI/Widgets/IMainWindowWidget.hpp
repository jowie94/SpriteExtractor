#pragma once

#include <vector>
#include <memory>

#include "IWidget.hpp"
#include "IPanelWidget.hpp"
#include "UI/PopupsController.hpp"

class IMainWindowWidget : public IWidget
{
public:
	explicit IMainWindowWidget(const char* name);
    virtual ~IMainWindowWidget() = default;

protected:
	template<typename T, typename... Args>
	std::weak_ptr<T> AddPanel(Args&&... args)
	{
		static_assert(std::is_base_of<IPanelWidget, T>::value, "Can't convert T to IPanelWidget");
		std::shared_ptr<T> panel = std::make_shared<T>(std::forward<Args...>(args...));
		AddPanelInt(panel);
		return panel;
	}

	PopupsController& GetPopupsController();

private:
	void AddPanelInt(std::shared_ptr<IPanelWidget> panel);

	void BeginWidget() override final;
	void EndWidget() override final;

	void SetupLayout();

	std::vector<std::shared_ptr<IPanelWidget>> _panels;
	PopupsController _popupsController;
};
