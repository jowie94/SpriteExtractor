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
    ui->Color->setColor(_selectedColor);
    connect(ui->SpriteViewer, &QtUI::SpriteViewer::HoveredColor, this, &NewProjectDialog::OnColorSelected);
    connect(ui->SpriteViewer, &QtUI::SpriteViewer::PickedColor, this, &NewProjectDialog::OnColorPicked);
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

    _originalColor = _selectedColor;

    // TODO: Custom color dialog?
    QColorDialog colorDialog(_selectedColor, this);
    connect(&colorDialog, &QColorDialog::colorSelected, this, &NewProjectDialog::OnColorSelected);
    colorDialog.exec();
}

void NewProjectDialog::OnPickerToggled(bool enabled)
{
    ui->SpriteViewer->SetPickColorEnabled(enabled);
}

void NewProjectDialog::OnColorPicked(QColor color)
{
    ui->TogglePickerButton->setChecked(false);

    OnColorSelected(color);
}

void NewProjectDialog::OnColorSelected(QColor color)
{
    _selectedColor = std::move(color);
    ui->Color->setColor(_selectedColor);
    update();
}
