#include "colordisplay.h"

#include <QPainter>

QtUI::ColorDisplay::ColorDisplay(QWidget* parent)
: QWidget(parent)
{
}

void QtUI::ColorDisplay::setColor(QColor newColor)
{
    _currentColor = newColor;
}

void QtUI::ColorDisplay::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    painter.fillRect(rect(), _currentColor);

    painter.setPen(QPen(Qt::black));
    painter.drawRect(rect());
}
