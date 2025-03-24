#ifndef QCUSTOMBUTTON_H
#define QCUSTOMBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class qcustombutton : public QPushButton
{
    Q_OBJECT

public:
    explicit qcustombutton(QWidget *parent = nullptr);

    // Override sizeHint to return our desired size
    QSize sizeHint() const override;

protected:
    // Override paintEvent to customize appearance
    void paintEvent(QPaintEvent *event) override;
};

#endif // QCUSTOMBUTTON_H
