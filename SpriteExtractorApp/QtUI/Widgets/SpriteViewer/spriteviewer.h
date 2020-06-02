#pragma once

#include <QWidget>

#include "Services/AssetManager/AssetPtr.hpp"

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

        void SetPickColorEnabled(bool enabled);

    signals:
        void HoveredColor(QColor color);
        void PickedColor(QColor color);

    protected:
        void mouseMoveEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent* event) override;

    private:
        AssetPtr<QPixmap> _pixmap;
        QColor _hoveredColor;

        float _scale = 0.0f;
        bool _pickColor = false;
    };

}  // namespace QtUI
