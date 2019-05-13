#include "PanelWindow.hpp"

PanelWindow::PanelWindow(const char* name, const ImVec2& initialSize, ImGuiWindowFlags flags)
: BaseWindow(name, flags)
, _initialSize(initialSize)
{
}

void PanelWindow::RequestFocus()
{
	_requestFocus = true;
}

PanelWindow::ClosePolicy PanelWindow::GetClosePolicy() const
{
	return _closePolicy;
}

bool PanelWindow::IsOpened() const
{
	return _opened;
}

void PanelWindow::Close()
{
	assert(_closePolicy != ClosePolicy::NoClose);
	_opened = false;
}

void PanelWindow::SetClosePolicy(ClosePolicy policy)
{
	_closePolicy = policy;
}

bool PanelWindow::BeginWidget()
{
	bool* opened = _closePolicy != ClosePolicy::NoClose ? &_opened : nullptr;
    ImGui::SetNextWindowSize(_initialSize, ImGuiCond_FirstUseEver);

	if (_requestFocus)
	{
		ImGui::SetNextWindowFocus();
		_requestFocus = false;
	}

    return ImGui::Begin(GetName(), opened, _flags);
}

void PanelWindow::EndWidget(bool wasDrawn)
{
    ImGui::End();
}
