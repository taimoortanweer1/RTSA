#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UILayout();
    CoreInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::CoreInit()
{
    // Create worker objects
    m_receiver = new UDPReceiver(8); // Using port 12345
    m_writer = new FileWriter();
    m_processor = new DataProcessor;

    m_receiverThread = new QThread;
    m_writerThread = new QThread;


    // Move workers to threads
    m_receiver->moveToThread(m_receiverThread);
    m_writer->moveToThread(m_writerThread);

    // Connect signals and slots
    connect(m_receiverThread, &QThread::finished, m_receiver, &QObject::deleteLater);
    connect(m_receiverThread, &QThread::started, m_receiver, &UDPReceiver::start);

    connect(m_writerThread, &QThread::finished, m_writer, &QObject::deleteLater);
    connect(m_writerThread, &QThread::finished, m_writer, &QObject::deleteLater);

    // Setup flow
    connect(m_receiver, &UDPReceiver::rawDataReady,m_processor, &DataProcessor::dataProcessingReady);
    connect(m_processor, &DataProcessor::spectrumDataReady ,m_plotter, &Plotter::updatePlots);


    // Start threads
    m_receiverThread->start();
    m_writerThread->start();


}



void MainWindow::UILayout()
{
    /**
     * @brief adding plot to the central frame layout of the mainwindow UI
     */
    QHBoxLayout *layout = new QHBoxLayout(ui->framePlots);
    m_plotter = new Plotter(ui->framePlots);
    m_plotter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_plotter);
    ui->framePlots->setLayout(layout);

    /**
     * @brief adding connections of UI components to their corresponding slots
     */
    connect(ui->toolButtonPeaksSelect,    &QToolButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(UserSelectIndex::PeakSelect);});
    connect(ui->toolButtonBandScanSelect, &QToolButton::clicked, this, [this]() { ui->stackedWidget->setCurrentIndex(UserSelectIndex::BandSelect);});
    connect(ui->toolButtonDensity, &QToolButton::clicked, this, [this]() { m_plotter->setPlotMode(PlotMode::Density);  });
    connect(ui->toolButtonSpectrum, &QToolButton::clicked, this, [this]() { m_plotter->setPlotMode(PlotMode::Spectrum); });
    connect(ui->toolButtonDensitySpectrum, &QToolButton::clicked, this, [this]() { m_plotter->setPlotMode(PlotMode::SpectrumDensity);  });


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

}
