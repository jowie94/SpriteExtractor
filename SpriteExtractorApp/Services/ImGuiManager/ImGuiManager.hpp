#pragma once

class ImGuiManager
{
public:
    virtual ~ImGuiManager() = default;

    void Init();

    virtual void Update();

private:

};
