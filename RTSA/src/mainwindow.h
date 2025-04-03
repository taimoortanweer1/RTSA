#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Plotter.h"
#include "UDPReceiver.h"
#include "FileWriter.h"
#include "DataProcessor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


enum  UserSelectIndex {
    PeakSelect = 0,
    BandSelect
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
private:
    Ui::MainWindow *ui;
    Plotter *m_plotter;
    QThread *m_receiverThread;
    QThread *m_writerThread;
    QThread *m_plottingThread;

    UDPReceiver *m_receiver;
    FileWriter *m_writer;
    DataProcessor *m_processor;
    void UILayout();

    void resizeEvent(QResizeEvent *event);
    void CoreInit();
};
#endif // MAINWINDOW_H
