#include "materials.h"
namespace Graphics {
Material Materials::createMaterial( QVector<QStringRef>& all_lines, int& i, QString& freshMaterialName)
{
    Material new_material;
    new_material.name = freshMaterialName;

    for (; i < all_lines.size(); ++i)
    {
        QStringRef line = all_lines.at(i);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::SkipEmptyParts, Qt::CaseInsensitive);

        if (tokens.at(0) == "Ns")
            new_material.ns = tokens.at(1).toFloat();
        else if (tokens.at(0) == "Ka")
            new_material.ka = Color( tokens.at(1).toFloat(), tokens.at(2).toFloat(), tokens.at(3).toFloat());
        else if (tokens.at(0) == "Kd")
            new_material.kd = Color( tokens.at(1).toFloat(), tokens.at(2).toFloat(), tokens.at(3).toFloat());
        else if (tokens.at(0) == "Ks")
            new_material.ks = Color( tokens.at(1).toFloat(), tokens.at(2).toFloat(), tokens.at(3).toFloat());
        else if (tokens.at(0) == "Ke")
            new_material.ke = Color( tokens.at(1).toFloat(), tokens.at(2).toFloat(), tokens.at(3).toFloat());
        else if (tokens.at(0) == "Ni")
            new_material.ni = tokens.at(1).toFloat();
        else if (tokens.at(0) == "d")
            new_material.d = tokens.at(1).toFloat();
        else if (tokens.at(0) == "illum")
            new_material.illum = tokens.at(1).toInt();
        else if (tokens.at(0) == "map_Kd")
        {
            QString fileName;
            for (int i = 1; i < tokens.size(); ++i)
                fileName.append( tokens.at(i).toString() + ' ');          // The texture file name may contain spaces, so I join it all up to a string.
            fileName.chop(1);                                             // Remove the last space.
            new_material.map_Kd_fileName = fileName;
            new_material.map_Kd = createOpenGLTextureFromFile( fileName, true);
            new_material.useTexture = true;
        }
        else if (tokens.at(0) == "newmtl")
        {
            --i;        // Go one line back, so that it will be re-read in the "loadMaterials" function.
            break;
        }
    }
    return new_material;
}

Materials::Materials()
{
    //m_shaders = shaders;
}

QImage Materials::imageFromFile( QString fileName)
{
    return Shared::global.fileReader.readImageFile("textures/" + fileName);
}

void Materials::setTextureOnMaterial( QString materialName, QString textureName)
{
    // Refer to texture in material
    m_materials[materialName].map_Kd = Shared::graphics.textures.value( textureName);
    m_materials[materialName].useTexture = true;
}

void Materials::insertMaterial(Material &material)
{
    m_materials.insert( material.name, material);
}

GLuint Materials::createOpenGLTextureFromFile(QString fileName, bool readAlpha)
{
    QImage readImage = imageFromFile( fileName);

    if (readAlpha)
        readImage = readImage.convertToFormat(QImage::Format_ARGB32);
    else
        readImage = readImage.convertToFormat(QImage::Format_RGB32);

    return Shared::graphics.createOpenGLTexture( readImage);
}

void Materials::loadMaterials( const QString &mtlData)
{
    qDebug() << "Parsing mtl file...";
    QVector<QStringRef> all_lines = mtlData.splitRef('\n', QString::KeepEmptyParts, Qt::CaseInsensitive);

    for (int i = 0; i < all_lines.size(); ++i)
    {
        QStringRef line = all_lines.at(i);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::SkipEmptyParts, Qt::CaseInsensitive);     // Split line to tokens

        if (tokens.at(0) == "newmtl")                                                                   // A material definition starts with "newmtl".
        {
            ++i;
            QString new_material_name = tokens.at(1).toString();
            Material new_material = createMaterial( all_lines, i, new_material_name);
            m_materials.insert( new_material.name, new_material);
        }
    }
}

void Materials::createMaterial(QString name, Color color, float alpha)
{
    Material new_material;
    new_material.name = name;
    new_material.kd = color;
    new_material.d = alpha;
    m_materials.insert( new_material.name, new_material);
}

void Materials::applyMaterial(QString name)
{
    Material* material = &m_materials[name];

    // Texture
    if (material->useTexture)
    {
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, material->map_Kd);
        Shared::graphics.shaders.setUniformInteger("u_sampler", 0);
        return;
    }

    // Vertex color
    Shared::graphics.shaders.setUniformVec4( "u_color", material->kd, material->d);
}

Material* Materials::getMaterialByIndex( int key)
{
    QMap<QString,Material>::iterator it = m_materials.begin();
    it += key;
    return &it.value();
}

int Materials::size()
{
    return m_materials.size();
}

}
