#include "spriteviewer.h"

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
            float ratio = static_cast<float>(pixRect.width()) / static_cast<float>(pixRect.height());
            if (pixRect.width() > pixRect.height())
            {
                float width = std::min(pixRect.width(), widgetRect.width());
                float height = width / ratio;
                pixRect.setWidth(static_cast<int>(std::floor(width)));
                pixRect.setHeight(static_cast<int>(std::floor(height)));
            }
            else
            {
                float height = std::min(pixRect.height(), widgetRect.height());
                float width = height * ratio;
                pixRect.setWidth(static_cast<int>(std::floor(width)));
                pixRect.setHeight(static_cast<int>(std::floor(height)));
            }

            painter.drawPixmap(pixRect, *_pixmap);
        }

        // painter.drawLine(1, 5, 1, 10);
    }
}  // namespace QtUI
