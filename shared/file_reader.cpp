#include "file_reader.h"

namespace Shared {
FileReader::FileReader()
{}

void FileReader::setRootPath( QString& path)
{
    m_rootPath = path;
}

QImage FileReader::readImageFile( QString fileName)
{
    QFile qfile( m_rootPath + fileName);
    qDebug() << "Opening image file:" << m_rootPath + fileName;

    if (!qfile.exists())
    {
        qDebug() << "ERROR: " << fileName << " does not exist!";
        return QImage();
    }

    qfile.open( QIODevice::ReadOnly);
    QByteArray bytes = qfile.readAll();
    QBuffer buffer(&bytes);

    QFileInfo fileInfo (qfile);

    QImage result;
    QImageReader reader(&buffer, fileInfo.completeSuffix().toLatin1());
    reader.read(&result);
    return result;
}

QString FileReader::readTextFile(QString fileName)
{
    QFile qfile( m_rootPath + fileName);
    qDebug() << "Opening file:" << qPrintable(m_rootPath + fileName + "...");

    if (!qfile.exists())
    {
        qDebug() << "ERROR: " << fileName << " does not exist!";
        return QString();                                           // Return empty string.
    }

    qfile.open( QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&qfile);
    in.setCodec("UTF-8");
    return in.readAll();
}

QByteArray FileReader::readBinaryFile( QString fileName)
{
    QFile qfile( m_rootPath + fileName);
    qDebug() << "Opening file:" << m_rootPath + fileName;

    if (!qfile.exists())
    {
        qDebug() << "ERROR: " << fileName << " does not exist!";
        return QByteArray();
    }

    qfile.open( QIODevice::ReadOnly);
    return qfile.readAll();
}
}
