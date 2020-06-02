#include "huecolorselector.h"

#include <QPainter>

QtUI::HueColorSelector::HueColorSelector(QWidget* parent)
: QWidget(parent)
, _image(256, 256, QImage::Format_RGB32)
{
    QColor color;
    for (int hue = 0; hue < 256; ++hue)
    {
        for (int val = 0; val < 256; ++val)
        {
            color.setHsvF(static_cast<double>(hue) / 256, 1, 1.0 - (static_cast<double>(val) / 256));
            _image.setPixelColor(hue, val, color);
        }
    }
}

void QtUI::HueColorSelector::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    painter.drawImage(rect(), _image);

}

