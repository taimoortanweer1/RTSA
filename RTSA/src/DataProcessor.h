#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include "data.h"

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = nullptr);

public slots:
    void dataProcessingReady(const QByteArray &data);

signals:
    void spectrumDataReady(const QVector<double>& frequencies, const QVector<double>& amplitudes);
    void waterfallDataReady(const QVector<double>& frequencies, QVector<QVector<double>> &m_waterfallData);
private:
    int plotIndex;
    // FFT configuration
    double m_f1 = 40000000.0;    // 40 MHz
    double m_f2 = 5900000000.0;  // 5.9 GHz
    int m_fftSize = 8192;
    int m_currentBin = 0;

    QVector<double> m_fullSpectrumX;
    QVector<double> m_fullSpectrumY;
    //WATERFALL PLOT
    QVector<QVector<double>> m_waterfallData;
    int m_maxWaterfallLines = 200; // Adjust based on how many lines you want to keep

};

#endif // DATAPROCESSOR_H
