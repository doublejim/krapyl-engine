#ifndef GMATERIAL_H
#define GMATERIAL_H

#include "color.h"
#include "shaders.h"
#include "shared/graphics_shared.h"

#include <QDebug>
#include <QList>
#include <QMap>
#include <QImage>
#include <QVector>

namespace Graphics {
// =========== MATERIAL ===========

struct Material
{
    QString name;    // The name of the material

    float ns = 0.0;                 // Specular color weight
    Color kd = Color(1,1,1);        // Diffuse color
    Color ka = Color(1,1,1);        // Ambient color
    Color ks = Color(1,1,1);        // Specular color
    Color ke = Color(1,1,1);        // Emission
    float d = 1.0;                  // Transparency
    int illum = 0;                  // Illumination
    float ni = 0.0;                 // ???
    QString map_Kd_fileName;        // Diffuse texture filename

    GLuint map_Kd = 0;              // Diffuse texture
    QByteArray map_Kd_bytes;

    bool useTexture = false;
};

// =========== MATERIALS ===========

class Materials
{
public:
    Materials();
    QMap<QString, Material> m_materials;                // Color name, material.

    // ===== Create =====
    void loadMaterials( const QString &mtlData);
    void createMaterial( QString name, Color color, float alpha);
    void setTextureOnMaterial(QString materialName, QString textureName);
    void insertMaterial( Material& material);

    // ===== Render =====
    void applyMaterial(QString name);

    // ==================
    Material* getMaterialByIndex( int key);
    int size();

private:
    // ===== Create =====
    Material createMaterial(QVector<QStringRef> &all_lines, int &i, QString &freshMaterialName);
    GLuint createOpenGLTextureFromFile(QString fileName, bool readAlpha = true);
    //GLuint createOpenGLTexture(QImage &readImage);
    QImage imageFromFile(QString fileName);
};
}

#endif // GMATERIAL_H
