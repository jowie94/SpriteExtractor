#include "newprojectdialog.h"

#include <QFileDialog>

#include "Logger/Logger.hpp"
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent)
: QDialog(parent)
, ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::OnBrowse()
{
    std::unique_ptr<QFileDialog> fileDialog =
        std::make_unique<QFileDialog>(this);
    QString file = QFileDialog::getOpenFileName(
        this, tr("Open"), "", tr("Image Files (*.png);;All Files"));
    Logger::GetLogger("NewProject")
        ->debug("File selected {}", file.toStdString());

    ui->filePath->setText(file);
}
