#include "newprojectdialog.h"

#include <QFileDialog>
#include <QPushButton>
#include <QColorDialog>

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
    connect(ui->SpriteViewer, &QtUI::SpriteViewer::SelectedColor, this, &NewProjectDialog::OnColorSelected);
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

void NewProjectDialog::OnSelectColor()
{
    ui->TogglePickerButton->setChecked(false);

    QColor result = QColorDialog::getColor(_selectedColor, this);
    if (result.isValid())
    {
        UpdateSelectedColor(result);
    }
}

void NewProjectDialog::OnColorHovered(QColor color)
{
    if (ui->TogglePickerButton->isChecked())
    {   
        UpdateSelectedColor(color);
    }
}

void NewProjectDialog::OnColorSelected(QColor color)
{
    ui->TogglePickerButton->setChecked(false);

    UpdateSelectedColor(color);
}

void NewProjectDialog::UpdateSelectedColor(QColor color)
{
    _selectedColor = std::move(color);

    QPalette palette;
    palette.setColor(QPalette::Window, _selectedColor);
    ui->Color->setAutoFillBackground(true);
    ui->Color->setPalette(palette);
}
