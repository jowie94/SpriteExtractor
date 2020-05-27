#include "QtApp.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QProcess>

#include <Types.hpp>

#include "QtUI/mainwindow.h"

void QtApp::Run()
{
    QCoreApplication::setOrganizationName(QStringLiteral(""));

    int argc = 0;
    QApplication app(argc, nullptr);

    MainWindow mainWindow;
    mainWindow.show();

    QApplication::exec();
}

std::shared_ptr<IImage> QtApp::OpenImage(const std::string& path)
{
    return nullptr;
}
