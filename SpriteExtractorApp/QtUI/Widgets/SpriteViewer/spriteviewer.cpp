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
        //setMouseTracking(true);
    }

    void SpriteViewer::SetPixmap(AssetPtr<QPixmap> pixmap)
    {
        _pixmap = std::move(pixmap);
        CalculateScale();
    }

    void SpriteViewer::SetPickColorEnabled(bool enabled)
    {
        _pickColor = enabled;

        setMouseTracking(_pickColor);

        if (_pickColor)
        {
            setCursor(Qt::CrossCursor);
        }
        else
        {
            setCursor(Qt::ArrowCursor);
        }
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
        emit PickedColor(_hoveredColor);
    }

    void SpriteViewer::paintEvent(QPaintEvent* event)
    {
        QWidget::paintEvent(event);
        QPainter painter(this);

        if (_pixmap)
        {
            painter.drawPixmap(_pixmapRect, *_pixmap);
        }

        // painter.drawLine(1, 5, 1, 10);
    }

    void SpriteViewer::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);

        CalculateScale();
    }

    void SpriteViewer::CalculateScale()
    {
        if (_pixmap)
        {
            _pixmapRect = _pixmap->rect();
            QRect widgetRect = rect();
            float scaleX = static_cast<float>(widgetRect.width()) / static_cast<float>(_pixmapRect.width());
            float scaleY = static_cast<float>(widgetRect.height()) / static_cast<float>(_pixmapRect.height());
            _scale = std::min(scaleX, scaleY);

            _pixmapRect.setWidth(static_cast<int>(std::floor(static_cast<float>(_pixmapRect.width()) * _scale)));
            _pixmapRect.setHeight(static_cast<int>(std::floor(static_cast<float>(_pixmapRect.height()) * _scale)));
        }
    }
}  // namespace QtUI
