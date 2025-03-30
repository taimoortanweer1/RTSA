#include "PlotCanvas.h"
#include "ui_PlotCanvas.h"

PlotCanvas::PlotCanvas(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlotCanvas)
{
    ui->setupUi(this);
    setupUI();

    //customPlot = new QCustomPlot(this);
    //setCentralWidget(customPlot);
    // Setup the plot in a way that won't block
    //setupPlot();

    //PlotWorker *worker = new PlotWorker(this);
    // connect(worker, &PlotWorker::newDataReady, this, [this](const QVector<double> &x, const QVector<double> &y) {
    //     if (!customPlot->graphCount()) {
    //         customPlot->addGraph();
    //         customPlot->xAxis->setRange(0, 200);
    //         customPlot->yAxis->setRange(-1, 1);
    //     }
    //     customPlot->graph(0)->setData(x, y);
    //     customPlot->replot();
    // });

    m_receiver = new UDPReceiver(8); // Using port 12345

    connect(m_receiver, &UDPReceiver::spectrumDataReady, this, [this](const QVector<double> &x, const QVector<double> &y) {
        if (!customPlot->graphCount()) {
            customPlot->addGraph();
            customPlot->graph(0)->setPen(QPen(Qt::blue));
            customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));

            customPlot->xAxis->setLabel("Frequency (Hz)");
            customPlot->yAxis->setLabel("Amplitude");
            customPlot->xAxis->setNumberPrecision(0);
            customPlot->xAxis->setNumberFormat("g");
            customPlot->setBackground(Qt::black);

            // Set initial range (40MHz to 5.9GHz)
            customPlot->xAxis->setRange(0, 8000000000);
            customPlot->yAxis->setRange(-200, 300);  // Will auto-scale later
            customPlot->xAxis->setLabelColor(Qt::white);
            customPlot->yAxis->setLabelColor(Qt::white);
            customPlot->xAxis2->setLabelColor(Qt::yellow);
            customPlot->yAxis2->setLabelColor(Qt::cyan);
            customPlot->xAxis->setTickLabelColor(Qt::white);
            customPlot->yAxis->setTickLabelColor(Qt::white);
            customPlot->graph(0)->setAntialiased(false); // Disable antialiasing for the first graph


            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        }

        if (x.size() != x.size() || x.isEmpty()) {
            return;
        }

        customPlot->graph(0)->setData(x, y);

        // Auto-scale Y axis
        // auto yMinMax = std::minmax_element(y.constBegin(), y.constEnd());
        // customPlot->yAxis->setRange(*yMinMax.first, *yMinMax.second);


        customPlot->replot();
    });

    //worker->start();



    // Create worker objects

    m_receiverThread = new QThread;


    // Move workers to threads
    m_receiver->moveToThread(m_receiverThread);

    // Connect signals and slots
    connect(m_receiverThread, &QThread::finished, m_receiver, &QObject::deleteLater);


    // Setup UDP receiver
    // connect(m_receiver, &UDPReceiver::spectrumDataReady,
    //         m_plotter, &Plotter::updateSpectrum);

    connect(m_receiverThread, &QThread::started, m_receiver, &UDPReceiver::start);

    // Start threads
    m_receiverThread->start();
}

PlotCanvas::~PlotCanvas()
{
    delete ui;
}


void PlotCanvas::setupUI() {
    // Create a central widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create button panel
    QWidget *buttonPanel = new QWidget;
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonPanel);

    // Create buttons
    btnZoomIn = new QPushButton("Zoom In");
    btnZoomOut = new QPushButton("Zoom Out");
    btnReset = new QPushButton("Reset View");
    btnZoomIn1 = new QPushButton("Zoom In1");
    btnZoomOut1 = new QPushButton("Zoom Out1");
    btnReset1 = new QPushButton("Reset View1");
    // Add buttons to button panel
    buttonLayout->addWidget(btnZoomIn);
    buttonLayout->addWidget(btnZoomOut);
    buttonLayout->addWidget(btnReset);
    buttonLayout->addWidget(btnZoomIn1);
    buttonLayout->addWidget(btnZoomOut1);
    buttonLayout->addWidget(btnReset1);
    // Create plot
    customPlot = new QCustomPlot;

    // Add widgets to main layout
    mainLayout->addWidget(buttonPanel);
    mainLayout->addWidget(customPlot);

    // Set central widget
    setCentralWidget(centralWidget);

    // Connect button signals
    connect(btnZoomIn, &QPushButton::clicked, [this]() {
        customPlot->xAxis->scaleRange(0.8);
        customPlot->yAxis->scaleRange(0.8);
        customPlot->replot();
    });

    connect(btnZoomOut, &QPushButton::clicked, [this]() {
        customPlot->xAxis->scaleRange(1.2);
        customPlot->yAxis->scaleRange(1.2);
        customPlot->replot();
    });

    connect(btnReset, &QPushButton::clicked, [this]() {
        customPlot->rescaleAxes();
        customPlot->replot();
    });
}
