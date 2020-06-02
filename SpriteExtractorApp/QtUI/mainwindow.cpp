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
    OnOpenNewProject(); // TODO: TMP
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::OnOpenNewProject()
{
    Logger::GetLogger("Main Window")->debug("Import Clicked");

    NewProjectDialog projectDialog(this);
    connect(&projectDialog, &QDialog::accepted, this, &MainWindow::OnNewProject);
    projectDialog.exec();
}

void MainWindow::OnNewProject()
{
    Logger::GetLogger("Main Window")->debug("New Project");
}

void MainWindow::setupConnections()
{
    connect(ui->actionNewProject, &QAction::triggered, this, &MainWindow::OnOpenNewProject);
}
