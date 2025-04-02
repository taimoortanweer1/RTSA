#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "lib\qcustomplot.h"
#include "data.h"
class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);
    void setPlotMode(PlotMode mode);

signals:

public slots:
    void updateSpectrum (const QVector<double> &frequencies, const QVector<double> &amplitudes);
    void updateWaterfall(const QVector<double> &frequencies, const QVector<double> &amplitudes);
    void updatePlots(const QVector<double> &frequencies, const QVector<double> &amplitudes);

private:

    QCPColorMap              *m_colorMap;
    QCPColorScale            *m_colorScale;
    int                      m_maxTimePoints = 10;
    QVector<double>          m_frequencies;
    QVector<QVector<double>> m_data;
    PlotMode                 m_plotMode;
    QCustomPlot              *m_plot;

    void setupSpectrumPlot(QCPAxisRect *axisRect);
    void setupWaterfallPlot(QCPAxisRect *axisRect);
    PlotMode getPlotMode();
    void updatePlot();
    void setupPlot(int plotMode);
};

#endif // PLOTTER_H
