#include "FileWriter.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileWriter::FileWriter(QObject *parent) : QObject(parent)
{
}

void FileWriter::writeToFile(const QString &filename, const QVector<int32_t> &data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing:" << filename;
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < data.size(); ++i) {
        out << data[i];
        if (i != data.size() - 1) {
            out << ",";
        }
    }
    out << "\n";

    file.close();
    qDebug() << "File written:" << filename;
}
