#pragma once

#include <QWidget>

#include "Services/AssetManager/AssetLoader.hpp"

namespace QtUI
{
    namespace Ui
    {
        class SpriteViewer;
    }

    class SpriteViewer : public QWidget
    {
        Q_OBJECT

    public:
        explicit SpriteViewer(QWidget* parent = nullptr);
        ~SpriteViewer() = default;

        void SetPixmap(AssetPtr<QPixmap> pixmap);

    signals:
        void HoveredColor(QColor color);

    protected:
        void mouseMoveEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent* event) override;

    private:
        AssetPtr<QPixmap> _pixmap;

        float _scale = 0.0f;
    };

}  // namespace QtUI
