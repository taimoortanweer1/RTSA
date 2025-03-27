#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>
#include <QVector>

class FileWriter : public QObject
{
    Q_OBJECT

public:
    explicit FileWriter(QObject *parent = nullptr);

public slots:
    void writeToFile(const QString &filename, const QVector<int32_t> &data);
};

#endif // FILEWRITER_H
