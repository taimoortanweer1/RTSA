#include "DataProcessor.h"
#include <QDebug>
DataProcessor::DataProcessor(QObject *parent)
    : QObject{parent}
{
    m_fullSpectrumX.reserve(m_fftSize);
    m_fullSpectrumY.reserve(m_fftSize);

}

void DataProcessor::dataProcessingReady(const QByteArray &data)
{

    const int32_t *packetData = reinterpret_cast<const int32_t*>(data.constData());
    const int numInts = data.size() / sizeof(int32_t);

    // Extract metadata from header (first 8 int32)
    const int packetCounter = packetData[1];
    const int packetIndex = packetData[2];
    const int totalPackets = packetData[3];
    const int fftSize = packetData[4];



    //FFT PLOT
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

    //emit waterfallDataReady(m_fullSpectrumX, m_waterfallData); // New signal to update waterfall



}
