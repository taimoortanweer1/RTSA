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
    void setupPlot();
    QCustomPlot *m_plot;

signals:
     void replotNeeded();

public slots:
    void updateSpectrum(const QVector<double>& frequencies, const QVector<double>& amplitudes);

private:


};

#endif // PLOTTER_H
