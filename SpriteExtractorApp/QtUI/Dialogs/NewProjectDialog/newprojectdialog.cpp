#include "newprojectdialog.h"

#include <QFileDialog>

#include "QtUI/Assets/QPixmapLoader.hpp"

#include "Logger/Logger.hpp"
#include "Services/AssetManager/AssetManager.hpp"
#include "Services/Services.hpp"

#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget* parent)
: QDialog(parent)
, ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    connect(ui->SpriteViewer, &QtUI::SpriteViewer::HoveredColor, this, &NewProjectDialog::OnColorHovered);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

void NewProjectDialog::OnBrowse()
{
    std::unique_ptr<QFileDialog> fileDialog = std::make_unique<QFileDialog>(this);
    QString file = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("Image Files (*.png);;All Files"));
    Logger::GetLogger("NewProject")->debug("File selected {}", file.toStdString());

    ui->filePath->setText(file);

    AssetPtr<QPixmap> pixmap = Services::GetInstance().Get<AssetManager>()->GetAsset<QPixmap>(file.toStdString());
    ui->SpriteViewer->SetPixmap(pixmap);
    ui->SpriteViewer->repaint();
}

void NewProjectDialog::OnColorHovered(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Window, color);
    ui->Color->setAutoFillBackground(true);
    ui->Color->setPalette(palette);
}
