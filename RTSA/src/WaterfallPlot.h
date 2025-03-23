#ifndef WATERFALLPLOT_H
#define WATERFALLPLOT_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "lib\qcustomplot.h"

class WaterfallPlot : public QWidget
{
    Q_OBJECT
public:
    explicit WaterfallPlot(QWidget *parent = nullptr);
    QCustomPlot *customPlot;
    QCPColorMap *colorMap;
    QVector<QVector<double>> data;

public slots:
    void updateData();
signals:
private:
};

#endif // WATERFALLPLOT_H
