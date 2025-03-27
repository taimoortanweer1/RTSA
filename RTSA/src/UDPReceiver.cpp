#include "UDPReceiver.h"
#include <QNetworkDatagram>
#include <QFile>
#include <QDebug>

UDPReceiver::UDPReceiver(quint16 port, QObject *parent)
    : QObject(parent), m_port(port), m_running(false),
    m_writeBuffer(8200), m_writeBufferIndex(0),
    m_packetCounter(0), m_dropCounter(0),
    m_samplesPerPacket(256), m_fftLength(0),
    m_totalFFTs(0), m_fileNumber(1)
{
    m_socket = new QUdpSocket(this);
}

UDPReceiver::~UDPReceiver()
{
    stop();
}

void UDPReceiver::start()
{
    if (m_running) return;

    if (!m_socket->bind(QHostAddress::Any, m_port)) {
        emit errorOccurred(tr("Failed to bind to port %1").arg(m_port));
        return;
    }

    connect(m_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
    m_running = true;
    qDebug() << "UDP receiver started on port" << m_port;
}

void UDPReceiver::stop()
{
    if (!m_running) return;

    m_socket->close();
    disconnect(m_socket, &QUdpSocket::readyRead, this, &UDPReceiver::readPendingDatagrams);
    m_running = false;
    qDebug() << "UDP receiver stopped";
}

void UDPReceiver::readPendingDatagrams()
{
    while (m_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_socket->receiveDatagram();
        if (!datagram.isValid()) {
            emit errorOccurred("Invalid datagram received");
            continue;
        }

        QByteArray data = datagram.data();
        if (data.size() < 32) { // Minimum header size (8 int32_t values)
            emit errorOccurred("Datagram too small");
            continue;
        }

        const int32_t *receivedData = reinterpret_cast<const int32_t*>(data.constData());
        quint32 currentPacketCounter = static_cast<quint32>(receivedData[1]);

        // Check for packet drops
        if (m_packetCounter != 0 && m_packetCounter != currentPacketCounter) {
            m_dropCounter += (currentPacketCounter - m_packetCounter);
            emit packetDropped(m_packetCounter, currentPacketCounter, m_dropCounter);
        }
        m_packetCounter = currentPacketCounter + 1;

        // Extract metadata
        m_fftLength = static_cast<quint32>(receivedData[4]);
        m_totalFFTs = static_cast<quint32>(receivedData[5]);
        m_fileNumber = static_cast<quint32>(receivedData[6]);

        // Process data payload (skip 8 int32_t header)
        int dataItems = (data.size() - 32) / sizeof(int32_t);
        QVector<int32_t> payload(dataItems);
        memcpy(payload.data(), receivedData + 8, dataItems * sizeof(int32_t));

        // Emit signal with received data
        emit dataReceived(payload, currentPacketCounter);

        // Buffer data for file writing
        if (m_writeBufferIndex + dataItems < static_cast<quint32>(m_writeBuffer.size())) {
            memcpy(m_writeBuffer.data() + m_writeBufferIndex, payload.data(), dataItems * sizeof(int32_t));
            m_writeBufferIndex += dataItems;
        } else {
            emit errorOccurred("Write buffer overflow");
        }

        // Check if this is the last packet
        if (receivedData[2] == (receivedData[3] + 1)) {
            quint32 totalSamples = m_fftLength * m_totalFFTs;
            QString filename = QString("%1.csv").arg(m_fileNumber);

            // Ensure we don't exceed buffer bounds
            if (totalSamples > static_cast<quint32>(m_writeBuffer.size())) {
                totalSamples = m_writeBuffer.size();
            }

            QVector<int32_t> fileData(totalSamples);
            memcpy(fileData.data(), m_writeBuffer.data(), totalSamples * sizeof(int32_t));
            emit fileReady(filename, fileData);

            // Reset buffer for next file
            m_writeBufferIndex = 0;
        }
    }
}
