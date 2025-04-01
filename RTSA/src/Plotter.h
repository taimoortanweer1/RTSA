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
    QCustomPlot *m_plot;

signals:
     void replotNeeded();

public slots:
    void updateSpectrum (const QVector<double> &frequencies, const QVector<double> &amplitudes);
    void updateWaterfall(const QVector<double> &frequencies, const QVector<double> &amplitudes);

private:

    void setupSpectrumPlot();
    void setupWaterfallPlot();

    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
    int m_maxTimePoints = 10;
    QVector<double> m_frequencies;
    QVector<QVector<double>> m_data;
    void updatePlot();
};

#endif // PLOTTER_H
