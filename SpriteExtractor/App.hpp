#include <string>

class App
{
public:
    virtual void Run() = 0;

protected:
    void Loop();

private:
    std::string selectedFile;
};