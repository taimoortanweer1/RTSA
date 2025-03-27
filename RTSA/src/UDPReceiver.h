#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QVector>

class UDPReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UDPReceiver(quint16 port, QObject *parent = nullptr);
    ~UDPReceiver();

    void start();
    void stop();

signals:
    void dataReceived(const QVector<int32_t> &data, quint32 packetCounter);
    void fileReady(const QString &filename, const QVector<int32_t> &data);
    void errorOccurred(const QString &error);
    void packetDropped(quint32 expected, quint32 received, quint32 totalDrops);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *m_socket;
    quint16 m_port;
    bool m_running;
    QVector<int32_t> m_writeBuffer;
    quint32 m_writeBufferIndex;
    quint32 m_packetCounter;
    quint32 m_dropCounter;
    quint32 m_samplesPerPacket;
    quint32 m_fftLength;
    quint32 m_totalFFTs;
    quint32 m_fileNumber;
};

#endif // UDPRECEIVER_H
