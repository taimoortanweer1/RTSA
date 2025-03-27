// udpreceiver.h
#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QUdpSocket>
#include <QObject>
#include <QVector>

class UDPReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UDPReceiver(quint16 port, QObject *parent = nullptr);
    void start();
    void stop();

signals:
    void spectrumDataReady(const QVector<double>& frequencies, const QVector<double>& amplitudes);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *m_socket;
    quint16 m_port;
    bool m_running;

    // FFT configuration
    double m_f1 = 40000000.0;    // 40 MHz
    double m_f2 = 5900000000.0;  // 5.9 GHz
    int m_fftSize = 8192;
    int m_currentBin = 0;

    QVector<double> m_fullSpectrumX;
    QVector<double> m_fullSpectrumY;
};

#endif // UDPRECEIVER_H
