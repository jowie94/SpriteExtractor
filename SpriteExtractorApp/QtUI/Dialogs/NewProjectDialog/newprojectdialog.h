#pragma once

#include <QDialog>

namespace Ui
{
    class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget* parent = nullptr);
    ~NewProjectDialog();

private slots:
    void OnBrowse();
    void OnSelectColor();

    void OnColorHovered(QColor color);
    void OnColorSelected(QColor color);

    void UpdateSelectedColor(QColor color);

private:
    Ui::NewProjectDialog* ui;

    QColor _selectedColor;
};
