#include "IWidget.hpp"

#include <cassert>

IWidget::IWidget(const char* name, ImGuiWindowFlags flags)
: _flags(flags)
, _name(name)
{
}

void IWidget::DoDraw()
{
	BeforeDraw();
	BeginWidget();
	Draw();
	assert(_drawn && "Base IWidget::Draw() not called, are you calling the parent Draw()?");
	EndWidget();
	AfterDraw();
	_drawn = false;
}

void IWidget::Init()
{
	assert(!_init);
	_init = true;
}

const char* IWidget::GetName() const
{
	return _name;
}

void IWidget::Draw()
{
	assert(_init && "Drawing a not initialized window");
	_drawn = true;
}
