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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UILayout()
{
    /**
     * @brief adding plot to the central frame layout of the mainwindow UI
     */
    QHBoxLayout *layout = new QHBoxLayout(ui->framePlots);
    m_waterfallPlot = new WaterfallPlot(ui->framePlots);
    m_waterfallPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_waterfallPlot);
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
