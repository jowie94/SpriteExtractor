#include "mainwindow.h"

#include <QFileDialog>

#include "Dialogs/NewProjectDialog/newprojectdialog.h"
#include "ui_mainwindow.h"

#include "Logger/Logger.hpp"

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::OnNewProject()
{
    Logger::GetLogger("Main Window")->debug("Import Clicked");

    NewProjectDialog projectDialog(this);
    projectDialog.exec();
}

void MainWindow::setupConnections()
{
    connect(ui->actionNewProject, &QAction::triggered, this, &MainWindow::OnNewProject);
}
