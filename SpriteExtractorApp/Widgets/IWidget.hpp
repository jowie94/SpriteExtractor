#pragma once

class IWidget
{
public:
    virtual ~IWidget() = default;

    virtual void Init() = 0;
    virtual void Draw() = 0;
};
