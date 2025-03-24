#include "qcustombutton.h"
#include <QPushButton>
#include <QPainter>
#include <QColor>

qcustombutton::qcustombutton(QWidget *parent)
    : QPushButton{parent}
{

    // Set fixed size to 50x50
    setFixedSize(50, 50);

    // Set size policy to fixed to prevent resizing
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Optional: Remove focus rectangle
    setFocusPolicy(Qt::NoFocus);

    // Set stylesheet for circular appearance
    setStyleSheet(
        "QPushButton {"
        "   border-radius: 25px;"  // Half of width/height
        "   border: 1px solid #8f8f91;"
        "   min-width: 50px;"
        "   max-width: 50px;"
        "   min-height: 50px;"
        "   max-height: 50px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #d0d0d0;"
        "}"
        );
}

QSize qcustombutton::sizeHint() const
{
    // Return the desired size (50x50)
    return QSize(50, 50);
}

void qcustombutton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

}
