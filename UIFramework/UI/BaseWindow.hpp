#pragma once

#include <ImGui/imgui.h>

class BaseWindow
{
public:
    BaseWindow(const char* name, ImGuiWindowFlags);
    virtual ~BaseWindow() = default;

    void DoDraw();

    virtual void Init();

    const char* GetName() const;

protected:
    virtual void BeforeDraw() {}
    virtual void BeginWidget() = 0;
    virtual void Draw();
    virtual void EndWidget() = 0;
    virtual void AfterDraw() {}

    ImGuiWindowFlags _flags;

private:
    bool _init = false;
    bool _drawn = false;

    const char* _name = nullptr;
};
