#include "Plotter.h"

Plotter::Plotter(QWidget *parent)
    : QWidget{parent}
{
    // Set up the QCustomPlot widget
    // Set up the QCustomPlot widget
    customPlot = new QCustomPlot(this);

    // Use a layout to manage the QCustomPlot widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
    setLayout(layout);

    // Ensure the widget expands to fill its parent
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Initialize the data storage
    data.resize(10); // 10 seconds depth
    for (int i = 0; i < 10; ++i) {
        data[i].resize(100); // 100 data points per second
    }

    // Set up the plot
    customPlot->xAxis->setLabel("Frequency (GHz)");
    customPlot->yAxis->setLabel("Time (s)");
    customPlot->xAxis->setRange(5, 6);
    customPlot->yAxis->setRange(0, 10);
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis2->setLabelColor(Qt::white);
    customPlot->yAxis2->setLabelColor(Qt::white);

    customPlot->setBackground(Qt::black);

    // Set up a color map
    colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
    colorMap->data()->setSize(100, 10); // 100 frequency points, 10 time steps
    colorMap->data()->setRange(QCPRange(5, 6), QCPRange(0, 10));
    colorMap->setGradient(QCPColorGradient::gpJet);

    // Add a color scale
    QCPColorScale *colorScale = new QCPColorScale(customPlot);
    customPlot->plotLayout()->addElement(0, 1, colorScale);
    colorScale->setType(QCPAxis::atRight);
    colorMap->setColorScale(colorScale);
    colorScale->axis()->setLabel("Magnitude");


    // Set up a timer to simulate data acquisition
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Plotter::updateData);
    timer->start(1000); // Update every second

}
void Plotter::updateData()
{
    // Simulate new data acquisition
    QVector<double> newData(100);
    for (int i = 0; i < 100; ++i) {
        newData[i] = (double)rand() / RAND_MAX; // Random magnitude between 0 and 1
    }

    // Shift data down and add new data
    data.pop_back();
    data.push_front(newData);

    // Update the color map
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 100; ++j) {
            colorMap->data()->setCell(j, i, data[i][j]);
        }
    }

    // Replot the graph
    customPlot->replot();
}
