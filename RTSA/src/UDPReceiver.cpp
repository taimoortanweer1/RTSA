// udpreceiver.cpp
#include "udpreceiver.h"
#include <QNetworkDatagram>

UDPReceiver::UDPReceiver(quint16 port, QObject *parent)
    : QObject(parent), m_port(port), m_running(false)
{
    m_socket = new QUdpSocket(this);
    m_fullSpectrumX.reserve(m_fftSize);
    m_fullSpectrumY.reserve(m_fftSize);
}

void UDPReceiver::start()
{
    if (!m_running && m_socket->bind(QHostAddress::Any, m_port)) {
        connect(m_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
        m_running = true;
        m_currentBin = 0;
        m_fullSpectrumX.clear();
        m_fullSpectrumY.clear();
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

        const int32_t *packetData = reinterpret_cast<const int32_t*>(data.constData());
        const int numInts = data.size() / sizeof(int32_t);

        // Extract metadata from header (first 8 int32)
        const int packetCounter = packetData[1];
        const int packetIndex = packetData[2];
        const int totalPackets = packetData[3];
        const int fftSize = packetData[4];

        // Calculate frequency bin width
        const double binWidth = (m_f2 - m_f1) / fftSize;

        // Process FFT data (starting after 8-int header)
        for (int i = 8; i < numInts; i++) {
            double frequency = m_f1 + (binWidth * m_currentBin++);
            m_fullSpectrumX.append(frequency);
            m_fullSpectrumY.append(packetData[i]);  // Raw amplitude
        }

        // If we've received all packets for this FFT frame
        if (packetIndex == totalPackets + 1) {
            emit spectrumDataReady(m_fullSpectrumX, m_fullSpectrumY);
            m_currentBin = 0;
            m_fullSpectrumX.clear();
            m_fullSpectrumY.clear();
        }
    }
}
