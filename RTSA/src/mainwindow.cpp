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
    start();
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

    // Move workers to threads
    m_receiver->moveToThread(&m_receiverThread);
    m_writer->moveToThread(&m_writerThread);

    // Connect signals and slots
    connect(&m_receiverThread, &QThread::finished, m_receiver, &QObject::deleteLater);
    connect(&m_writerThread, &QThread::finished, m_writer, &QObject::deleteLater);
    connect(m_receiver, &UDPReceiver::fileReady, m_writer, &FileWriter::writeToFile);
    connect(m_receiver, &UDPReceiver::dataReceived, this, [](const QVector<int32_t> &data, quint32 counter) {
        qDebug() << "Received packet" << counter << "with" << data.size() << "samples";
    });
    connect(m_receiver, &UDPReceiver::packetDropped, this, [](quint32 expected, quint32 received, quint32 total) {
        qWarning() << "Packet drop - Expected:" << expected << "Received:" << received << "Total drops:" << total;
    });
    connect(m_receiver, &UDPReceiver::errorOccurred, this, [](const QString &error) {
        qWarning() << "Error:" << error;
    });

    // Start threads
    m_receiverThread.start();
    m_writerThread.start();
}

void MainWindow::start()
{
    QMetaObject::invokeMethod(m_receiver, &UDPReceiver::start);
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


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

}
