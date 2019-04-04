#include "BaseWindow.hpp"

#include <cassert>

BaseWindow::BaseWindow(const char* name, ImGuiWindowFlags flags)
: _flags(flags)
, _name(name)
{
}

void BaseWindow::DoDraw()
{
	BeforeDraw();
	BeginWidget();
	Draw();
	assert(_drawn && "Base IWidget::Draw() not called, are you calling the parent Draw()?");
	EndWidget();
	AfterDraw();
	_drawn = false;
}

void BaseWindow::Init()
{
	assert(!_init);
	_init = true;
}

const char* BaseWindow::GetName() const
{
	return _name;
}

void BaseWindow::Draw()
{
	assert(_init && "Drawing a not initialized window");
	_drawn = true;
}
