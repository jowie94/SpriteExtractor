#pragma once

#include <QWidget>

namespace QtUI
{
    class ColorDisplay : public QWidget
    {
        Q_OBJECT;

    public:
        ColorDisplay(QWidget* parent);

        void setColor(QColor newColor);

        void paintEvent(QPaintEvent* paintEvent) override;

    private:
        QColor _currentColor;
    };
}
