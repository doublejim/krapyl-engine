#include "mesh_loader.h"

namespace Graphics {
void MeshLoader::resetForNextMaterial( GLuint& materialIndexValueLowest, GLuint& materialIndexValueHighest, GLuint& materialIndexStart, GLuint& objectIndex)
{
    // Reset highest and lowest index values.
    materialIndexValueLowest = UINT_MAX;
    materialIndexValueHighest = 0;
    materialIndexStart = objectIndex;
}

void MeshLoader::updateObjectExtremes( uint* objectIndexValueOffset, GLuint& materialIndexValueHighest, GLuint& materialTexIndexValueHighest, GLuint& materialNormalIndexValueHighest)
{
    if (objectIndexValueOffset[0] < materialIndexValueHighest)          // objectIndexValueHighest
        objectIndexValueOffset[0] = materialIndexValueHighest;

    if (objectIndexValueOffset[1] < materialTexIndexValueHighest)       // objectTexIndexValueHighest
        objectIndexValueOffset[1] = materialTexIndexValueHighest;

    if (objectIndexValueOffset[2] < materialNormalIndexValueHighest)    // objectNormalIndexValueHighest
        objectIndexValueOffset[2] = materialNormalIndexValueHighest;
}

void MeshLoader::loadObject( Mesh* target, QVector<QStringRef>& obj_all_lines, int& currentLine, uint* globalIndexValueOffset, bool loadTexCoords, bool loadNormals)
{
    m_target = target;
    m_target->m_loading = true;

    QVector<GLfloat> foundPositions, foundNormals, foundTexCoords;
    QVector<uint> foundPositionIndices, foundTexIndices, foundNormalIndices;

    uint objectIndexValueOffset[3] = {0, 0, 0};

    // VERTICES

    for (; currentLine < obj_all_lines.size(); ++currentLine)
    {
        QStringRef line = obj_all_lines.at(currentLine);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::SkipEmptyParts, Qt::CaseInsensitive);

        if (tokens.at(0) == "v")
        {
            for (QStringRef value : tokens.mid(1, 3))
                foundPositions.append( value.toFloat());
        } else break;
    }

    // VERTEX TEXTURE COORDINATES

    for (; currentLine < obj_all_lines.size(); ++currentLine)
    {
        QStringRef line = obj_all_lines.at(currentLine);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::SkipEmptyParts, Qt::CaseInsensitive);

        if (tokens.at(0) == "vt")
        {
            if (!loadTexCoords)         // This is bad.
                continue;

            for (QStringRef value : tokens.mid(1, 2))
                foundTexCoords.append( value.toFloat());
        } else break;
    }

    // VERTEX NORMALS

    for (; currentLine < obj_all_lines.size(); ++currentLine)
    {
        QStringRef line = obj_all_lines.at(currentLine);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::SkipEmptyParts, Qt::CaseInsensitive);

        if (tokens.at(0) == "vn")
        {
            if (!loadNormals)           // This is bad.
                continue;

            for (QStringRef value : tokens.mid(1, 3))
                foundNormals.append( value.toFloat());
        } else break;
    }

    // PREPARE FOR LOADING INDICES / MATERIALS

    bool hasDrawMode = false;
    bool attributeIndicesChecked = false;
    bool hasTextureCoordIndices = false;
    bool hasNormalIndices = false;
    GLuint objectIndex = 0;                             // The number of index values that have been traversed.
    GLuint materialIndexStart = 0;                      // The index value when the material starts being applied.
    GLuint materialIndexValueLowest = UINT_MAX;         // The lowest value of an index in the current material.
    GLuint materialIndexValueHighest = 0;               // The highest value of a position index in the current material.
    GLuint materialTexIndexValueHighest = 0;            // The highest value of a texture index in the current material.
    GLuint materialNormalIndexValueHighest = 0;         // The highest value of a normal index in the current material.
    QString materialName;

    // INDICES / MATERIALS LOADER

    for (; currentLine < obj_all_lines.size(); ++currentLine)
    {
        QStringRef line = obj_all_lines.at(currentLine);

        if (line.isEmpty())
            continue;

        QVector<QStringRef> tokens = line.split(' ', QString::SkipEmptyParts, Qt::CaseInsensitive);

        if (tokens.at(0) == 's' || tokens.at(0) == 'g')
        {
            continue;

        // STORE ON WHICH INDICES WE MUST APPLY WHAT MATERIALS

        } else if (tokens.at(0) == "usemtl") {

            // Applying all materials except the last. It's not known whether a material is
            // last until there are no more lines. That's why the last updateMaterialIndices() is
            // at the end of the function.
            // We always update the PREVIOUS material, when we are told to use a new one.

            // Update object extremes if material extremes are more extreme. (extreme)

            updateObjectExtremes( objectIndexValueOffset, materialIndexValueHighest, materialTexIndexValueHighest, materialNormalIndexValueHighest);

            // objectIndex minus one, because at the end of finding indices, we added one, to prepare for saving the next index.

            if (!materialName.isEmpty())
                m_target->m_materialRanges.append( materialName, materialIndexStart, objectIndex - 1, materialIndexValueLowest, materialIndexValueHighest);

            materialName = tokens.at(1).toString();
            resetForNextMaterial( materialIndexValueLowest, materialIndexValueHighest, materialIndexStart, objectIndex);

            continue;

        // INDICES

        } else if (tokens.at(0) == 'f' || tokens.at(0) == 'l') {

            // Set draw mode according to how many tokens there are on the line.

            if (!hasDrawMode)
            {
                m_target->setDrawMode( tokens.size() - 1);
                hasDrawMode = true;
            }

            for (QStringRef set : tokens.mid(1)) // tokens.mid(1) example: {5/1/1, 1/2/1, 4/3/1, 6/4/1}
            {
                QVector<QStringRef> tuple = set.split('/', QString::KeepEmptyParts, Qt::CaseInsensitive);

                // example: tuple: {5,1,1}
                // 0 is vertex index, 1 is texture coordinate index, 2 is normal index.

                if (!attributeIndicesChecked)
                {
                    if (tuple.size() > 1)
                        hasTextureCoordIndices = true;
                    if (tuple.size() > 2)
                        hasNormalIndices = true;
                    attributeIndicesChecked = true;
                }

                GLuint indexValue = tuple.at(0).toUInt() - 1 - globalIndexValueOffset[0];

                // example: indexValue: 5
                // I subtract 1 because the indices start at 1 in the .obj, but
                // must start at 0. See an explanation for globalIndexValueOffset at
                // the end of this function.

                foundPositionIndices.append( indexValue);

                // Update extremes.

                if (materialIndexValueLowest > indexValue)
                    materialIndexValueLowest = indexValue;

                if (materialIndexValueHighest < indexValue)
                    materialIndexValueHighest = indexValue;

                // Update texture indices.

                if (hasTextureCoordIndices)
                {
                    GLuint textureIndexValue = tuple.at(1).toUInt() - 1 - globalIndexValueOffset[1];

                    if (materialTexIndexValueHighest < textureIndexValue)
                        materialTexIndexValueHighest = textureIndexValue;

                    if (loadTexCoords)
                        foundTexIndices.append( textureIndexValue);
                }

                // Update normal indices.

                if (hasNormalIndices)
                {
                    GLuint normalIndexValue = tuple.at(2).toUInt() - 1 - globalIndexValueOffset[2];

                    if (materialNormalIndexValueHighest < normalIndexValue)
                        materialNormalIndexValueHighest = normalIndexValue;

                    if (loadNormals)
                        foundNormalIndices.append( normalIndexValue);
                }

                ++objectIndex;
            }
            continue;
        } else if (tokens.at(0) == 'o') {       // There's a new object, so end this one!

            // Go back one character, so that the object may be rediscovered in the entity loader.
            --currentLine;
            break;
        } else {
            qDebug() << "ERROR: unknown token in obj file:" << tokens.at(0) << " at line:" << currentLine;
            continue;
        }
    }

    updateObjectExtremes( objectIndexValueOffset, materialIndexValueHighest, materialTexIndexValueHighest, materialNormalIndexValueHighest);

    // Minus one, because at the end of finding indices, we added one, to prepare for saving the next index.

    if (!materialName.isEmpty())
        m_target->m_materialRanges.append( materialName, materialIndexStart, objectIndex - 1, materialIndexValueLowest, materialIndexValueHighest);

    // Now, that all the data has been loaded nicely, we need to know how to arrange it.
    // In 'expandData' we determine if we are going to use indices to 'not repeat data' unnecessarily.
    // If we use textures on the object, we can't use indices, and values will need to be doubled.
    // If there are only positions, we will use indices.
    // If there are only positions and normals, it will check if we will gain from using indices,
    // and use them if so.

    expandData( foundPositions, foundPositionIndices, foundTexCoords, foundTexIndices, foundNormals, foundNormalIndices);

    QList<QVector<GLfloat>> attributesResult;
    attributesResult.append( foundPositions);
    attributesResult.append( foundNormals);
    attributesResult.append( foundTexCoords);

    QList<QByteArray> attributeNames = {"a_position", "a_normal", "a_texCoord0"};
    QList<GLuint> attribComponents = {3, 3, 2};

    m_target->feedWithData( attributesResult, attributeNames, attribComponents, foundPositionIndices);

    // Blender exports indices of SEPARATE OBJECTS (within the same obj file) with a continuous + 1 in offset.
    // The indexValue must always start at 0.
    // The globalIndexValueOffsets are used to make the indexValues (for each index-type) start at 0 no matter which
    // object in the .obj we are at.

    globalIndexValueOffset[0] += objectIndexValueOffset[0] + 1;
    globalIndexValueOffset[1] += objectIndexValueOffset[1] + 1;
    globalIndexValueOffset[2] += objectIndexValueOffset[2] + 1;
}

void MeshLoader::expandData( QVector<GLfloat>& positions, QVector<uint>& positionIndices, QVector<GLfloat>& texCoords, QVector<uint>& texCoordIndices, QVector<GLfloat>& normals, QVector<uint>& normalIndices)
{
    // EXPAND DATA. There are several possible scenarios:
    //
    // - There are only vertices and vertexIndices.
    //   We just use the already collected indices and quit.
    //
    // - If there are textureCoords we can't use indices.
    //   So all the vertices are expanded.
    //
    // - If there are just vertices and normals, we need to test.
    //   The collected vertexRanges don't apply if we've made new indices.
    //   So these need to be recalculated.

    QVector<GLfloat> expandedPositions;
    QVector<GLfloat> expandedTexCoords;
    QVector<GLfloat> expandedNormals;
    QVector<uint> properPositionIndices;

    Mode mode = UseIndices;                                     // Use indices if there are only vertices and positionIndices.

    if (!texCoords.isEmpty())                                   // If there are texture coordinates.
        mode = DontUseIndices;                                  // All data will need to be expanded.
    else if (normals.isEmpty() || positionIndices.isEmpty())    // If there are no texture coordinates and no normals OR there are no position indices.
        mode = UseIndices;
    else
        mode = PerhapsIndices;                                  // If there are normals, but no texture coordinates.
                                                                // MAYBE we want to use indices.
                                                                // Below we will calculate the gain of using indices.

    switch(mode)
    {
    case Graphics::MeshLoader::UseIndices:
    {
        // Vertices, positionIndices and m_materialRanges are all perfect as they are.
        m_target->m_useIndices = true;
        return;
    }
    case Graphics::MeshLoader::DontUseIndices:
    {
        // We're using textures, so we can't use indices. Expand all vertices.
        for (int i = 0; i < positionIndices.size(); ++i)
            for (int j = 0; j < 3; ++j)
            {
                expandedPositions.append( positions.at( 3 * positionIndices[i] + j));
                expandedNormals.append( normals.at( 3 * normalIndices[i] + j));

                if (j < 2)
                    expandedTexCoords.append( texCoords.at( 2 * texCoordIndices[i] + j));
            }
        //m_target->m_useTexture = true;
        m_target->m_useIndices = false;
        break;
    }
    case Graphics::MeshLoader::PerhapsIndices:
    {
        // We're using positions and normals. We shall calculate the gain of using indices, and
        // apply them if there is gain.
        determineWhetherToUseIndices( positions, positionIndices, normals, normalIndices,
                                      expandedPositions, expandedNormals, properPositionIndices);
        break;
    }
    }

    if (m_target->m_useIndices)
        qDebug() << "Using indices.";
    else
        qDebug() << "Not using indices.";

    // Give all the results back through the references.

    positions = expandedPositions;
    texCoords = expandedTexCoords;
    normals = expandedNormals;
    positionIndices = properPositionIndices;
}

void MeshLoader::determineWhetherToUseIndices( QVector<GLfloat>& positions,         QVector<uint>& positionIndices,
                                               QVector<GLfloat>& normals,           QVector<uint>& normalIndices,
                                               QVector<GLfloat>& expandedPositions, QVector<GLfloat>& expandedNormals,
                                               QVector<uint>& properPositionIndices)
{
    // Find all unique combinations of position and normal.

    QMap<QString,int> uniquePositionNormalCombos;

    for (int i = 0; i < positionIndices.size(); ++i)
        uniquePositionNormalCombos.insert( QString::number(positionIndices[i]) + '/' + QString::number(normalIndices[i]), -1);

    // Calculate the memory cost.
    // - WITH indices: there is the array of indices, and the 2 shortened arrays 'positions'
    //   and 'normals' times '3 values per vertex'.
    // - WITHOUT indices: there are 2 full length arrays 'positions' and 'normals' times
    //   '3 values per vertex'.

    int withIndices = 2 * 3 * uniquePositionNormalCombos.size() + positionIndices.size();
    int withoutIndices = 2 * 3 * positionIndices.size();
    float gainRatio = 1.0 * withIndices / withoutIndices;

    qDebug() << "Gain ratio from using indices:" << qPrintable( QString::number(gainRatio, 'f', 2));

    if (gainRatio >= Shared::global.indexGainCondition)
    {   // If true, there isn't enough gain from using indices, so we won't.
        // Instead, we will expand to the max.

        for (int i = 0; i < positionIndices.size(); ++i)
            for (int j = 0; j < 3; ++j)
            {
                expandedPositions.append( positions.at( 3 * positionIndices[i] + j));
                expandedNormals.append( normals.at( 3 * normalIndices[i] + j));
            }

        m_target->m_useIndices = false;

    } else {
        // There's gain from using indices, so we'll expand to each unique combination of position and normal.

        int materialRangeIndex = 0;

        // Get the first range out, and reset extremes.

        MaterialRange* materialRange = &m_target->m_materialRanges.list[materialRangeIndex];
        materialRange->indexValueLowest = UINT_MAX;
        materialRange->indexValueHighest = 0;

        // Go through all the indices. Remember each unique combination of vertex and normal
        // that we've been told to draw. We reuse the 'uniquePositionNormalCombos' that
        // was used to calculate the cost of using indices vs not.
        // Then add the index to the positionIndices whenever we must draw that combination.

        for (int i = 0; i < positionIndices.size(); ++i)
        {
            QString key = QString::number(positionIndices[i]) + '/' + QString::number(normalIndices[i]);

            if (uniquePositionNormalCombos.value(key) == -1)
            {   // If not yet used combination: insert position and normal.
                GLuint updatedIndexValue = expandedPositions.size() / 3;

                // The combination is now being used.
                uniquePositionNormalCombos.insert( key, updatedIndexValue);

                // The index existed, but we need to tell that we're using it again.
                properPositionIndices.append( GLuint(uniquePositionNormalCombos.value(key)));

                // Insert the values which the index will refer to.

                for (int j = 0; j < 3; ++j)
                    expandedPositions.append( positions.at( 3 * positionIndices[i] + j));

                for (int j = 0; j < 3; ++j)
                    expandedNormals.append( normals.at( 3 * normalIndices[i] + j));

                // Update extremes.

                if (materialRange->indexValueLowest > updatedIndexValue)
                    materialRange->indexValueLowest = updatedIndexValue;

                if (materialRange->indexValueHighest < updatedIndexValue)
                    materialRange->indexValueHighest = updatedIndexValue;

            } else {
                // Reuse an already stored combination.
                properPositionIndices.append( GLuint(uniquePositionNormalCombos.value(key)));
            }
            // Next we'll fix the material range extremes.
            // When the end range of a material has been met, we will reset its extremes,
            // because this whole function changes the index values, so the extremes must be found again.

            if (materialRange->indexEnd == i)
            {
                // Below: don't pick one after the last.
                if (++materialRangeIndex < m_target->m_materialRanges.list.size())
                {
                    materialRange = &m_target->m_materialRanges.list[materialRangeIndex];

                    // Reset extremes.
                    materialRange->indexValueLowest = UINT_MAX;
                    materialRange->indexValueHighest = 0;
                }
            }
        }
        m_target->m_useIndices = true;
    }
}
}
