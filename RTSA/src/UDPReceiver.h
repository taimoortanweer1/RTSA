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
    void rawDataReady(const QByteArray &data);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *m_socket;
    quint16 m_port;
    bool m_running;


};

#endif // UDPRECEIVER_H
