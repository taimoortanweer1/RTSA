// udpreceiver.cpp
#include "udpreceiver.h"
#include <QNetworkDatagram>

UDPReceiver::UDPReceiver(quint16 port, QObject *parent)
    : QObject(parent), m_port(port), m_running(false)
{
    m_socket = new QUdpSocket(this);

}

void UDPReceiver::start()
{
    if (!m_running && m_socket->bind(QHostAddress::Any, m_port)) {
        connect(m_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
        m_running = true;

    }
}

void UDPReceiver::stop()
{
    if (m_running) {
        m_socket->close();
        disconnect(m_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
        m_running = false;
    }
}

void UDPReceiver::readPendingDatagrams()
{
    while (m_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_socket->receiveDatagram();
        if (!datagram.isValid()) continue;

        const QByteArray data = datagram.data();
        if (data.size() != 1056) continue;  // Expecting 1056-byte packets

        emit rawDataReady(data);

    }
}
