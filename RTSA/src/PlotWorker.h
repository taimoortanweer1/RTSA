// plotworker.h
#include <QThread>
#include <QVector>
#include <QMutex>

class PlotWorker : public QThread {
    Q_OBJECT
public:
    explicit PlotWorker(QObject *parent = nullptr);
    void stop();

signals:
    void newDataReady(const QVector<double> &x, const QVector<double> &y);

protected:
    void run() override;

private:
    QMutex mutex;
    bool m_stop = false;
};
