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

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        AssetPtr<QPixmap> _pixmap;
    };

}  // namespace QtUI
