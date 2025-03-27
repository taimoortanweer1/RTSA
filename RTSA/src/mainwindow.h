#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/Plotter.h"
#include "src/UDPReceiver.h"
#include "src/FileWriter.h"

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

private:
    Ui::MainWindow *ui;
    Plotter *m_plotter;
    QThread m_receiverThread;
    QThread m_writerThread;
    UDPReceiver *m_receiver;
    FileWriter *m_writer;

    void UILayout();

    void resizeEvent(QResizeEvent *event);
    void CoreInit();
    void start();
};
#endif // MAINWINDOW_H
