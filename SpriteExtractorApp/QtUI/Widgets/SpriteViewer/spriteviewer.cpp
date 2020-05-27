#include "spriteviewer.h"

#include <cmath>

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include "Logger/Logger.hpp"

namespace QtUI
{
    SpriteViewer::SpriteViewer(QWidget* parent)
    : QWidget(parent)
    {
        setMouseTracking(true);
    }

    void SpriteViewer::SetPixmap(AssetPtr<QPixmap> pixmap)
    {
        _pixmap = std::move(pixmap);
    }

    void SpriteViewer::mouseMoveEvent(QMouseEvent* event)
    {
        QWidget::mouseMoveEvent(event);

        QPoint pos = (event->localPos() / _scale).toPoint();
        if (_pixmap && pos.y() < _pixmap->size().height() && pos.x() < _pixmap->size().width())
        {
            //Logger::GetLogger("SpriteViewer")->debug("Mouse over {},{}", pos.x(), pos.y());
            _hoveredColor = _pixmap->toImage().pixel(pos);
            emit HoveredColor(_hoveredColor);
        }
    }

    void SpriteViewer::mousePressEvent(QMouseEvent* /*event*/)
    {
        emit SelectedColor(_hoveredColor);
    }

    void SpriteViewer::paintEvent(QPaintEvent* event)
    {
        QWidget::paintEvent(event);
        QPainter painter(this);

        if (_pixmap)
        {
            QRect pixRect = _pixmap->rect();
            QRect widgetRect = rect();
            float scaleX = static_cast<float>(widgetRect.width()) / static_cast<float>(pixRect.width());
            float scaleY = static_cast<float>(widgetRect.height()) / static_cast<float>(pixRect.height());
            _scale = std::min(scaleX, scaleY); // TODO: Only calculate on resize
            float width = static_cast<float>(pixRect.width()) * _scale;
            float height = static_cast<float>(pixRect.height()) * _scale;
            pixRect.setWidth(static_cast<int>(std::floor(static_cast<float>(width))));
            pixRect.setHeight(static_cast<int>(std::floor(static_cast<float>(height))));
            painter.drawPixmap(pixRect, *_pixmap);
        }

        // painter.drawLine(1, 5, 1, 10);
    }
}  // namespace QtUI
