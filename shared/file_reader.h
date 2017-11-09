#ifndef FILEREADER_H
#define FILEREADER_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QBuffer>
#include <QImageReader>

namespace Shared {
class FileReader
{
public:
    QString m_rootPath;
    FileReader();
    QString readTextFile(QString fileName);
    void setRootPath(QString &path);
    QImage readImageFile(QString fileName);
    QByteArray readBinaryFile( QString fileName);
};
}

#endif // FILEREADER_H
