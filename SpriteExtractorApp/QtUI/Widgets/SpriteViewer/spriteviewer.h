#pragma once

#include <QWidget>

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
        ~SpriteViewer();

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        class QPixmap* _pixmap = nullptr;
    };

}  // namespace QtUI
