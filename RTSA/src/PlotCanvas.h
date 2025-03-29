#ifndef PLOTCANVAS_H
#define PLOTCANVAS_H

#include <QMainWindow>
#include "lib\qcustomplot.h"
#include "src\PlotWorker.h"
#include "src/UDPReceiver.h"
#include <QThread>
namespace Ui {
class PlotCanvas;
}

class PlotCanvas : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotCanvas(QWidget *parent = nullptr);
    ~PlotCanvas();

private:
    Ui::PlotCanvas *ui;
    QCustomPlot *customPlot;
    UDPReceiver *m_receiver;
    QThread *m_receiverThread;
    QPushButton *btnZoomIn, *btnZoomOut, *btnReset;
    QPushButton *btnZoomIn1, *btnZoomOut1, *btnReset1;

    void setupUI();
};

#endif // PLOTCANVAS_H
