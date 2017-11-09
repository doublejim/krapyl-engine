#include "entity.h"
namespace Graphics {
Entity::Entity() //Shaders *shaders):
    //m_materials(shaders)
{
    //m_shaders = shaders;
}

Entity::~Entity()
{
}

void Entity::render()
{
    saveMatrix();                   // Save rotation and translation as it is now.
//    applyTransform();               // Applies rotation and translation as it is in this object.
    applyTransform2();

    applyDepthTesting();

    //printMatrix();

    Shared::graphics.shaders.useProgram( m_shaderProgram);

    for (Mesh* mesh : m_meshes)
        mesh->render();

    Shared::graphics.shaders.endOfProgramUse();

    loadMatrix();                   // Resets the rotation and translation to what it was when it was saved.

    //glDepthMask(GL_TRUE);
}

void Entity::setShaderProgram( QString programName)
{
    m_shaderProgram = Shared::graphics.shaders.getProgram( programName);
}

void Entity::setDepthTestMode(DepthTestMode mode)
{
    m_depthTestMode = mode;
}

void Entity::applyDepthTesting()
{
    switch(m_depthTestMode)
    {
    case DepthTestLEQUAL:
        glDepthFunc(GL_LEQUAL);
        break;
    default: // disabled / always passes.
        glDepthFunc(GL_ALWAYS);
        break;
    }
}

void Entity::insertMesh(Graphics::Mesh *mesh)
{
    m_meshes.append( mesh);
}

void Entity::loadFromObjFile( QString fileName, bool loadTexCoords, bool loadNormals, CullingMode culling)
{
    QElapsedTimer timer;
    timer.start();

    qDebug() << "loading obj-file:" << qPrintable(fileName + "...");
    QString data = Shared::global.fileReader.readTextFile( "data/" + fileName);

    // Split the whole obj-file into lines:
    QVector<QStringRef> obj_all_lines = data.splitRef('\n', QString::KeepEmptyParts, Qt::CaseInsensitive);

    MeshLoader meshLoader;

    /* There can be more than one object in a .obj file.
     * When that is the case, each of the index types (position/texture/normal)
     * will never have the same index-values as the object before it.
     * So, we need to keep track of what the offset is, when loading the data
     * of an object. The globalIndexValueOffset will keep track of the index
     * offset for each index type.
     */

    uint globalIndexValueOffset[3] = {0,0,0};

    // This loop loads all of the mtl-files and objects referenced in the obj-file.
    // During one iteration (after finding an 'o') it will add one object from the obj.

    Materials* materials = Shared::graphics.createEmptyMaterials();

    for (int currentLine = 0; currentLine < obj_all_lines.size(); ++currentLine)
    {        
        QStringRef line = obj_all_lines.at(currentLine);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::KeepEmptyParts, Qt::CaseInsensitive);

        if (tokens.at(0) == "mtllib")
        {
            QString mtlFilename = tokens.at(1).toString();                              // ".mtl" is part of the read name.
            QString mtlData = Shared::global.fileReader.readTextFile( "data/"+ mtlFilename);
            materials->loadMaterials( mtlData);
            continue;
        }
        else if (tokens.at(0) == "o")                                                   // The "o"-line contains the name of the mesh, and is the start of an object defition.
        {
            Mesh* freshMesh = Shared::graphics.createEmptyMesh( materials);
            freshMesh->setCullingMode( culling);
            ++currentLine;
            freshMesh->m_loading = true;
            freshMesh->m_name = line.mid(2).toString();                                 // The name is the rest of the characters on the current line.
            qDebug() << "Loading object: " << freshMesh->m_name;
            meshLoader.loadObject( freshMesh, obj_all_lines, currentLine, globalIndexValueOffset, loadTexCoords, loadNormals);
            insertMesh( freshMesh);
            /*if (freshMesh->m_useIndices)
                qDebug() << qPrintable(freshMesh->m_name) << ":" << freshMesh->m_positionsCount << "positions /" << freshMesh->m_indexCount << "indices";
            else
                qDebug() << qPrintable(freshMesh->m_name) << ":" << freshMesh->m_positionsCount << "positions";*/
        }
        else if (line.startsWith('#'))
            continue;
    }
    qDebug() << "Time spent loading obj-file: " << timer.elapsed()/1000.0 << " seconds.";
}
}
