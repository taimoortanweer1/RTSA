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

public slots:
    void updateSpectrum(const QVector<double>& frequencies, const QVector<double>& amplitudes);

private:
    QCustomPlot *m_plot;

};

#endif // PLOTTER_H
