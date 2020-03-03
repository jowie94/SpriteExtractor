#include "spriteviewer.h"

#include <cmath>

#include <QLabel>
#include <QPainter>
#include <QPixmap>

namespace QtUI
{
    SpriteViewer::SpriteViewer(QWidget* parent)
    : QWidget(parent)
    {
    }

    void SpriteViewer::SetPixmap(AssetPtr<QPixmap> pixmap)
    {
        _pixmap = std::move(pixmap);
    }

    void SpriteViewer::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        if (_pixmap)
        {
            QRect pixRect = _pixmap->rect();
            QRect widgetRect = rect();
            float scaleX = static_cast<float>(widgetRect.width()) / static_cast<float>(pixRect.width());
            float scaleY = static_cast<float>(widgetRect.height()) / static_cast<float>(pixRect.height());
            float scale = std::min(scaleX, scaleY);
            float width = static_cast<float>(pixRect.width()) * scale;
            float height = static_cast<float>(pixRect.height()) * scale;
            pixRect.setWidth(static_cast<int>(std::floor(static_cast<float>(width))));
            pixRect.setHeight(static_cast<int>(std::floor(static_cast<float>(height))));
            painter.drawPixmap(pixRect, *_pixmap);
        }

        // painter.drawLine(1, 5, 1, 10);
    }
}  // namespace QtUI
