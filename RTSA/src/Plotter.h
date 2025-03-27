#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "lib\qcustomplot.h"

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);
    QCustomPlot *customPlot;
    QCPColorMap *colorMap;
    QVector<QVector<double>> data;

public slots:
    void updateData();
signals:
private:
};

#endif // PLOTTER_H
