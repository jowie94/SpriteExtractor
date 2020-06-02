#pragma once

#include <QWidget>

namespace QtUI
{
    class HueColorSelector : public QWidget
    {
        Q_OBJECT

    public:
        HueColorSelector(QWidget* parent);

        void paintEvent(QPaintEvent* event) override;

    private:
        QImage _image;
    };
}  // namespace QtUI