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


    setPlotMode(PlotMode::Density);
    setupPlot(m_plotMode);

    //setupWaterfallPlot();
    //setPlotMode(PlotMode::Density);
}


void Plotter::setupPlot(int plotMode = 0) {

    // m_plot->clearItems();

    // // Remove all layouts and axes if they exist
    // //m_plot->plotLayout()->clear();

    // if (plotMode == PlotMode::Spectrum) {
    //     // Spectrum plot only
    //     setupSpectrumPlot(nullptr);
    // }
    // else if (plotMode == PlotMode::Density) {
    //     // Waterfall plot only
    //     setupWaterfallPlot(nullptr);
    // }
    // else if (plotMode == PlotMode::SpectrumDensity) {
    //     // Both plots in vertical layout
    //     QCPLayoutGrid *topLevelLayout = new QCPLayoutGrid;
    //     m_plot->plotLayout()->addElement(topLevelLayout);

    //     // Spectrum plot at top
    //     QCPAxisRect *spectrumAxisRect = new QCPAxisRect(m_plot);
    //     topLevelLayout->addElement(0, 0, spectrumAxisRect);

    //     // Waterfall plot at bottom
    //     QCPAxisRect *waterfallAxisRect = new QCPAxisRect(m_plot);
    //     topLevelLayout->addElement(1, 0, waterfallAxisRect);

    //     setupWaterfallPlot(waterfallAxisRect);
    //     setupSpectrumPlot(spectrumAxisRect);
    // }


    // Create two plot layouts
    QCPLayoutGrid *mainLayout = new QCPLayoutGrid;
    QCPAxisRect *topAxisRect = new QCPAxisRect(m_plot);
    QCPAxisRect *bottomAxisRect = new QCPAxisRect(m_plot);

    // Remove the default axis rect
    m_plot->plotLayout()->clear();

    // Add the two axis rects to the main layout
    mainLayout->addElement(0, 0, topAxisRect);
    mainLayout->addElement(1, 0, bottomAxisRect);
    m_plot->plotLayout()->addElement(mainLayout);

    // Create graphs in each axis rect
    m_plot->addGraph(topAxisRect->axis(QCPAxis::atBottom), topAxisRect->axis(QCPAxis::atLeft));
    m_plot->addGraph(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));
    m_plot->setBackground(Qt::black);

    // Customize each graph
    m_plot->graph(0)->setPen(QPen(Qt::blue));
    m_plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    //WATERFALL
    m_plot->graph(1)->setPen(QPen(Qt::blue));
    m_plot->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    // Create color map
    m_colorMap = new QCPColorMap(bottomAxisRect->axis(QCPAxis::atBottom), bottomAxisRect->axis(QCPAxis::atLeft));

    // Set up axes
   bottomAxisRect->axis(QCPAxis::atBottom)->setLabel("Frequency (Hz)");
   bottomAxisRect->axis(QCPAxis::atLeft)->setLabel("Time");
   bottomAxisRect->axis(QCPAxis::atLeft)->setRangeReversed(true); // Newest at top

    // Add color scale
    m_colorScale = new QCPColorScale(m_plot);
    m_plot->plotLayout()->addElement(1, 1, m_colorScale);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorMap->setColorScale(m_colorScale);
    m_colorScale->axis()->setLabel("Amplitude");

    // Set color gradient
    m_colorMap->setGradient(QCPColorGradient::gpSpectrum);
    // Alternative gradients: gpHot, gpCold, gpNight, gpCandy, gpGeography

    // Set interactivity
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_plot->setBackground(Qt::black);


    // Set axis labels for each
    topAxisRect->axis(QCPAxis::atBottom)->setLabel("Top Graph X");
    topAxisRect->axis(QCPAxis::atLeft)->setLabel("Top Graph Y");
    bottomAxisRect->axis(QCPAxis::atBottom)->setLabel("Bottom Graph X");
    bottomAxisRect->axis(QCPAxis::atLeft)->setLabel("Bottom Graph Y");

}
PlotMode Plotter::getPlotMode()
{
    return m_plotMode;
}

void Plotter::setupWaterfallPlot( QCPAxisRect *axisRect = nullptr )
{

    if (!axisRect) {
        // axisRect = m_plot->axisRect(); // Use default axis rect if none provided
    }
    // Clear existing plots
    m_plot->addGraph();
    m_plot->graph(0)->setPen(QPen(Qt::blue));
    m_plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    m_plot->setBackground(Qt::black);

    // Create color map
    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);

    // Set up axes
    m_plot->xAxis->setLabel("Frequency (Hz)");
    m_plot->yAxis->setLabel("Time");
    m_plot->yAxis->setRangeReversed(true); // Newest at top

    // Add color scale
    m_colorScale = new QCPColorScale(m_plot);
    m_plot->plotLayout()->addElement(0, 1, m_colorScale);
    m_colorScale->setType(QCPAxis::atRight);
    m_colorMap->setColorScale(m_colorScale);
    m_colorScale->axis()->setLabel("Amplitude");

    // Set color gradient
    m_colorMap->setGradient(QCPColorGradient::gpSpectrum);
    // Alternative gradients: gpHot, gpCold, gpNight, gpCandy, gpGeography

    // Set interactivity
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void Plotter::setupSpectrumPlot(QCPAxisRect *axisRect = nullptr)
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

void Plotter::updateWaterfall(const QVector<double>& frequencies,
                              const QVector<double>& amplitudes)
{

    // Add new data to the buffer
    if (m_data.empty()) {
        m_frequencies = frequencies;
        m_data.resize(m_maxTimePoints);
        for (auto& row : m_data) {
            row.resize(frequencies.size());
        }
    }

    // Shift data down
    for (int i = m_maxTimePoints - 1; i > 0; --i) {
        m_data[i] = m_data[i - 1];
    }

    // Add new data at the top
    m_data[0] = amplitudes;

    updatePlot();

}

void Plotter::updatePlots(const QVector<double>& frequencies,
                          const QVector<double>& amplitudes)
{


    if (m_plotMode == PlotMode::Spectrum || m_plotMode == PlotMode::SpectrumDensity) {
        updateSpectrum(frequencies,amplitudes);
    }
    if (m_plotMode == PlotMode::Density || m_plotMode == PlotMode::SpectrumDensity) {
        updateWaterfall(frequencies,amplitudes);
    }


    m_plot->replot();
}

void Plotter::setPlotMode(PlotMode mode)
{
    m_plotMode = mode;
    setupPlot(m_plotMode);

}

void Plotter::updatePlot() {
    if (m_data.empty() || m_frequencies.empty()) return;

    const size_t nx = m_frequencies.size();
    const size_t ny = m_data.size();

    m_colorMap->data()->setSize(nx, ny);
    m_colorMap->data()->setRange(QCPRange(m_frequencies.first(), m_frequencies.last()),
                                 QCPRange(0, ny));

    // Find min/max for scaling
    double minVal = std::numeric_limits<double>::max();
    double maxVal = std::numeric_limits<double>::lowest();

    for (size_t y = 0; y < ny; ++y) {
        for (size_t x = 0; x < nx; ++x) {
            double val = m_data[y][x];
            m_colorMap->data()->setCell(x, y, val);

            if (val < minVal) minVal = val;
            if (val > maxVal) maxVal = val;
        }
    }

    // Set data range for color scaling
    m_colorMap->setDataRange(QCPRange(minVal, maxVal));

    m_plot->rescaleAxes();
   // m_plot->replot();
}
