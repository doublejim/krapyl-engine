#include "global_shared.h"
namespace Shared {

void Global::setApplicationPath(QString path)
{
    applicationPath = path;
    fileReader.setRootPath( path);
    qDebug() << "set application path:" << applicationPath;
}

float Global::fractionPart(float &value)
{
    return value - ((long)value);
}

void Global::setIndexGainCondition( float limit)
{
    indexGainCondition = limit;
}

void Global::init()
{
    matrixTransform = new Graphics::MatrixTransform();
    jsonSettings = new JsonWax();
    jsonKeyboard = new JsonWax();
}

Global global;
}
