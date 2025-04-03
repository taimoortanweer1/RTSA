#include "Plotter.h"

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{
    // Set up the QCustomPlot widget
    m_plot[PlotMode::Spectrum] = new QCustomPlot(this);
    m_plot[PlotMode::Density] = new QCustomPlot(this);

    // // Use a layout to manage the QCustomPlot widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_plot[PlotMode::Spectrum]);
    layout->addWidget(m_plot[PlotMode::Density]);

    setLayout(layout);

    //Ensure the widget expands to fill its parent
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //initialize
    setPlotMode(PlotMode::Spectrum);


}


void Plotter::setupPlot(int plotMode) {  // Remove default argument unless 0 is valid
    // Clear only the plots that will be used
    m_plot[PlotMode::Spectrum]->clearItems();
    m_plot[PlotMode::Density]->clearItems();

    // Hide both initially
    m_plot[PlotMode::Spectrum]->setVisible(false);
    m_plot[PlotMode::Density]->setVisible(false);

    switch(plotMode) {
    case PlotMode::Spectrum:
        m_plot[PlotMode::Spectrum]->setVisible(true);
        setupSpectrumPlot(m_plot[PlotMode::Spectrum]);
        break;
    case PlotMode::Density:
        m_plot[PlotMode::Density]->setVisible(true);
        setupWaterfallPlot(m_plot[PlotMode::Density]);
        break;
    case PlotMode::SpectrumDensity:
        m_plot[PlotMode::Spectrum]->setVisible(true);
        m_plot[PlotMode::Density]->setVisible(true);
        setupSpectrumPlot(m_plot[PlotMode::Spectrum]);
        setupWaterfallPlot(m_plot[PlotMode::Density]);
        break;
    default:
        // Handle invalid plotMode (log error or use default)
        break;
    }
}
PlotMode Plotter::getPlotMode()
{
    return m_plotMode;
}

void Plotter::setupWaterfallPlot(QCustomPlot *plot)
{

    m_plot[PlotMode::Density] = plot;
    // Clear existing plots
    if(m_plot[PlotMode::Density]->graphCount()==0)
        m_plot[PlotMode::Density]->addGraph();

    m_plot[PlotMode::Density]->graph(0)->setPen(QPen(Qt::blue));
    m_plot[PlotMode::Density]->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    m_plot[PlotMode::Density]->setBackground(Qt::black);

    // Create color map
    if(m_colorMap != nullptr)
        m_colorMap = new QCPColorMap(m_plot[PlotMode::Density]->xAxis, m_plot[PlotMode::Density]->yAxis);

    // Set up axes
    m_plot[PlotMode::Density]->xAxis->setLabel("Frequency (Hz)");
    m_plot[PlotMode::Density]->yAxis->setLabel("Time");
    m_plot[PlotMode::Density]->yAxis->setRangeReversed(true); // Newest at top


    m_plot[PlotMode::Density]->xAxis->setLabelColor(Qt::white);
    m_plot[PlotMode::Density]->yAxis->setLabelColor(Qt::white);
    m_plot[PlotMode::Density]->xAxis2->setLabelColor(Qt::yellow);
    m_plot[PlotMode::Density]->yAxis2->setLabelColor(Qt::cyan);
    m_plot[PlotMode::Density]->xAxis->setTickLabelColor(Qt::white);
    m_plot[PlotMode::Density]->yAxis->setTickLabelColor(Qt::white);
    m_plot[PlotMode::Density]->graph(0)->setAntialiased(false); // Disable antialiasing for the first graph




    // Add color scale
    if(m_colorScale != nullptr)
        m_colorScale = new QCPColorScale(m_plot[PlotMode::Density]);

    if(!m_plot[PlotMode::Density]->plotLayout()->elementAt(1))
        m_plot[PlotMode::Density]->plotLayout()->addElement(0, 1, m_colorScale);

    m_colorScale->setType(QCPAxis::atRight);
    m_colorMap->setColorScale(m_colorScale);
    m_colorScale->axis()->setLabel("Amplitude");

    // Set color gradient
    m_colorMap->setGradient(QCPColorGradient::gpSpectrum);
    // Alternative gradients: gpHot, gpCold, gpNight, gpCandy, gpGeography

    // Set interactivity
    m_plot[PlotMode::Density]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plotter::setupSpectrumPlot(QCustomPlot *plot)
{

    m_plot[PlotMode::Spectrum] = plot;

    if(m_plot[PlotMode::Spectrum]->graphCount()==0)
        m_plot[PlotMode::Spectrum]->addGraph();

    m_plot[PlotMode::Spectrum]->graph(0)->setPen(QPen(Qt::blue));
    m_plot[PlotMode::Spectrum]->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    m_plot[PlotMode::Spectrum]->xAxis->setLabel("Frequency (Hz)");
    m_plot[PlotMode::Spectrum]->yAxis->setLabel("Amplitude");
    m_plot[PlotMode::Spectrum]->xAxis->setNumberPrecision(0);
    m_plot[PlotMode::Spectrum]->xAxis->setNumberFormat("g");
    m_plot[PlotMode::Spectrum]->setBackground(Qt::black);

    // Set initial range (40MHz to 5.9GHz)
    m_plot[PlotMode::Spectrum]->xAxis->setRange(0, 8000000000);
    m_plot[PlotMode::Spectrum]->yAxis->setRange(0, 1000);  // Will auto-scale later
    m_plot[PlotMode::Spectrum]->xAxis->setLabelColor(Qt::white);
    m_plot[PlotMode::Spectrum]->yAxis->setLabelColor(Qt::white);
    m_plot[PlotMode::Spectrum]->xAxis2->setLabelColor(Qt::yellow);
    m_plot[PlotMode::Spectrum]->yAxis2->setLabelColor(Qt::cyan);
    m_plot[PlotMode::Spectrum]->xAxis->setTickLabelColor(Qt::white);
    m_plot[PlotMode::Spectrum]->yAxis->setTickLabelColor(Qt::white);
    m_plot[PlotMode::Spectrum]->graph(0)->setAntialiased(false); // Disable antialiasing for the first graph


    m_plot[PlotMode::Spectrum]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

}

void Plotter::updateSpectrum(const QVector<double>& frequencies, const QVector<double>& amplitudes)
{
    if (frequencies.size() != amplitudes.size() || frequencies.isEmpty()) {
        return;
    }
    m_plot[PlotMode::Spectrum]->graph(0)->data()->clear();  // Clears old data (optional)

    m_plot[PlotMode::Spectrum]->graph(0)->setData(frequencies, amplitudes);

    // Auto-scale Y axis
    auto yMinMax = std::minmax_element(amplitudes.constBegin(), amplitudes.constEnd());
    m_plot[PlotMode::Spectrum]->yAxis->setRange(*yMinMax.first, *yMinMax.second);



}

void Plotter::updateWaterfall(const QVector<double>& frequencies,
                              const QVector<double>& amplitudes)
{

    // Add new data to the buffer
    if (m_dataWaterfall.empty()) {
        m_frequencies = frequencies;
        m_dataWaterfall.resize(m_maxTimePoints);
        for (auto& row : m_dataWaterfall) {
            row.resize(frequencies.size());
        }
    }

    // Shift data down
    for (int i = m_maxTimePoints - 1; i > 0; --i) {
        m_dataWaterfall[i] = m_dataWaterfall[i - 1];
    }

    // Add new data at the top
    m_dataWaterfall[0] = amplitudes;

    updatePlotWaterfall();

}

void Plotter::updatePlots(const QVector<double>& frequencies,
                          const QVector<double>& amplitudes)
{


    if (m_plotMode == PlotMode::Spectrum || m_plotMode == PlotMode::SpectrumDensity) {
        updateSpectrum(frequencies,amplitudes);
        m_plot[PlotMode::Spectrum]->replot();
    }
    if (m_plotMode == PlotMode::Density || m_plotMode == PlotMode::SpectrumDensity) {
        updateWaterfall(frequencies,amplitudes);
        m_plot[PlotMode::Density]->replot();
    }



}

void Plotter::setPlotMode(PlotMode mode)
{
    m_plotMode = mode;
    setupPlot(m_plotMode);

}

void Plotter::updatePlotWaterfall() {
    if (m_dataWaterfall.empty() || m_frequencies.empty()) return;

    const size_t nx = m_frequencies.size();
    const size_t ny = m_dataWaterfall.size();

    m_colorMap->data()->setSize(nx, ny);
    m_colorMap->data()->setRange(QCPRange(m_frequencies.first(), m_frequencies.last()),
                                 QCPRange(0, ny));

    // Find min/max for scaling
    double minVal = std::numeric_limits<double>::max();
    double maxVal = std::numeric_limits<double>::lowest();

    for (size_t y = 0; y < ny; ++y) {
        for (size_t x = 0; x < nx; ++x) {
            double val = m_dataWaterfall[y][x];
            m_colorMap->data()->setCell(x, y, val);

            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
    }

    // Set data range for color scaling
    m_colorMap->setDataRange(QCPRange(minVal, maxVal));
    m_plot[PlotMode::Density]->rescaleAxes();
}
