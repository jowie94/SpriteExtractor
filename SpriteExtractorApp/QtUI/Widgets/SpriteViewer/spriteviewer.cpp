#include "spriteviewer.h"

#include <QPainter>
#include <QPixmap>

namespace QtUI
{
    SpriteViewer::SpriteViewer(QWidget* parent)
    : QWidget(parent)
    {
        _pixmap = new QPixmap(size());
    }

    SpriteViewer::~SpriteViewer()
    {
        delete _pixmap;
    }

    void SpriteViewer::paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);

        painter.drawPixmap(0, 0, *_pixmap);
    }
}  // namespace QtUI
