#include "SFMLApp.hpp"

int main()
{
    std::unique_ptr<App> app = std::make_unique<SFMLApp>();

    app->Run();

    return 0;
}
