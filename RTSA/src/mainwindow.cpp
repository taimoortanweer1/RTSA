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
    QHBoxLayout *layout = new QHBoxLayout(ui->framePlots);
    m_waterfallPlot = new WaterfallPlot(ui->framePlots);
    m_waterfallPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_waterfallPlot);
    ui->framePlots->setLayout(layout);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

}
