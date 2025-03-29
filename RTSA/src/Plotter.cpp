#include "Plotter.h"

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{
    // Set up the QCustomPlot widget
    m_plot = new QCustomPlot(this);

    // Use a layout to manage the QCustomPlot widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_plot);
    setLayout(layout);

    // Ensure the widget expands to fill its parent
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setupPlot();


}

void Plotter::setupPlot()
{
    m_plot->addGraph();
    m_plot->graph(0)->setPen(QPen(Qt::blue));
    m_plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    m_plot->xAxis->setLabel("Frequency (Hz)");
    m_plot->yAxis->setLabel("Amplitude");
    m_plot->xAxis->setNumberPrecision(0);
    m_plot->xAxis->setNumberFormat("g");
    m_plot->setBackground(Qt::black);

    // Set initial range (40MHz to 5.9GHz)
    m_plot->xAxis->setRange(0, 8000000000);
    m_plot->yAxis->setRange(0, 1000);  // Will auto-scale later
    m_plot->xAxis->setLabelColor(Qt::white);
    m_plot->yAxis->setLabelColor(Qt::white);
    m_plot->xAxis2->setLabelColor(Qt::yellow);
    m_plot->yAxis2->setLabelColor(Qt::cyan);
    m_plot->xAxis->setTickLabelColor(Qt::white);
    m_plot->yAxis->setTickLabelColor(Qt::white);
    m_plot->graph(0)->setAntialiased(false); // Disable antialiasing for the first graph


    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

}

void Plotter::updateSpectrum(const QVector<double>& frequencies, const QVector<double>& amplitudes)
{
    if (frequencies.size() != amplitudes.size() || frequencies.isEmpty()) {
        return;
    }
    m_plot->graph(0)->data()->clear();  // Clears old data (optional)

    m_plot->graph(0)->setData(frequencies, amplitudes);

    // Auto-scale Y axis
    auto yMinMax = std::minmax_element(amplitudes.constBegin(), amplitudes.constEnd());
    m_plot->yAxis->setRange(*yMinMax.first, *yMinMax.second);

    static int count = 0;

    if(count % 2 == 0)
        m_plot->replot();
    count++;

}
