// plotworker.cpp
#include "plotworker.h"

PlotWorker::PlotWorker(QObject *parent) : QThread(parent) {}

void PlotWorker::stop() {
    QMutexLocker locker(&mutex);
    m_stop = true;
}

void PlotWorker::run() {
    QVector<double> x(100), y(100);
    int offset = 0;

    while (!m_stop) {
        // Generate new data
        for (int i=0; i<100; ++i) {
            x[i] = i + offset;
            y[i] = qSin((i + offset)/10.0);
        }
        offset += 5;

        // Emit the new data
        emit newDataReady(x, y);

        // Sleep for a while
        msleep(50); // 20 updates per second
    }
}
