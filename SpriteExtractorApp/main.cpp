#include "QtApp.hpp"

int main()
{
    std::unique_ptr<App> app = std::make_unique<QtApp>();

    app->Run();

    return 0;
}

#ifdef WIN32
int WinMain()
{
    return main();
}
#endif
