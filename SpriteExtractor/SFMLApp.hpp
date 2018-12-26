#pragma once

#include "App.hpp"

class SFMLApp : public App
{
public:
    void Run() override;

protected:
    std::unique_ptr<IImage> OpenImage(const std::string& path) override;
};