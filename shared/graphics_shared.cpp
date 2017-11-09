#include "graphics_shared.h"

namespace Shared {
SharedGraphics::SharedGraphics()
{}

void SharedGraphics::deleteOpenGLContext()
{
    if (sdlOpenGLContext != nullptr)
        SDL_GL_DeleteContext( sdlOpenGLContext);
}

void SharedGraphics::createTexture(QString textureName, QImage &image, bool mipmap)
{
    GLuint tex = createOpenGLTexture( image, mipmap);
    Shared::graphics.textures.insert( textureName, tex);
}

void SharedGraphics::updateTexture(GLuint texName, QImage &updatedImage, QRect& rect)
{
    glBindTexture( GL_TEXTURE_2D, texName);
    glTexSubImage2D (GL_TEXTURE_2D, 0, rect.left(), rect.top(), rect.width(), rect.height(), GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, updatedImage.constBits());
}

GLuint SharedGraphics::createOpenGLTexture(QImage &readImage, bool mipmap)
{
    GLuint texName;
    glGenTextures( 1, &texName);
    glBindTexture( GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // GL_CLAMP_TO_EDGE  GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (Shared::global.jsonSettings->value({"mipmap_allowed"}).toBool() && mipmap)
    {
        int filtering = GL_LINEAR_MIPMAP_NEAREST;
        if (Shared::global.jsonSettings->value({"filtering"}, "bilinear").toString() == "bilinear")
            filtering = GL_LINEAR_MIPMAP_NEAREST;   // bi-linear
        else
            filtering = GL_LINEAR_MIPMAP_LINEAR;    // tri-linear

        // Automatic mipmapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    } else {
        // Without mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, readImage.width(), readImage.height(), 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, readImage.constBits()); // GL_UNSIGNED_INT_8_8_8_8_REV
    //glEnable(GL_TEXTURE_2D);
    return texName;
}

int SharedGraphics::createTexturedRect(Graphics::Point topLeft, Graphics::Point bottomRight, int subdivisions, Graphics::Color color, QString textureName, Shared::CullingMode culling)
{
    if (m_meshFactory == nullptr)
        m_meshFactory = new Graphics::MeshFactory();

    // Create new list of materials, and insert in materialsList.
    m_materialsList.append( Graphics::Materials());
    Graphics::Materials* materials = &m_materialsList.last();

    // Mesh
    Graphics::Mesh* new_mesh = m_meshFactory->generateTexturedRect( materials, topLeft, bottomRight, subdivisions);
    new_mesh->m_useIndices = false;
    createVBOFromMesh( new_mesh, VBOUsage::StaticDraw, VBOUsage::StaticDraw, FromVBOStart_InterleavedAttribs);
    // Indices should always be saved in one big VBO instead of this.

    // Material
    QString material_name = QString::number(0);
    materials->createMaterial( material_name, color, 1.0);
    materials->setTextureOnMaterial( material_name, textureName);
    int positionsCount = new_mesh->m_attribSizes.at(0) / 3;             // The first attribute is positions. Divide by 3 because colors are set per vertex.
    new_mesh->m_materialRanges.append( material_name, 0, positionsCount - 1, 0, positionsCount - 1);

    // Other options
    new_mesh->setCullingMode( culling);
    new_mesh->setDrawMode(3);
    m_meshes.append( new_mesh);

    return m_meshes.size() - 1;
}

Graphics::Mesh* SharedGraphics::createEmptyMesh( Graphics::Materials* materials)
{
    Graphics::Mesh* new_mesh = new Graphics::Mesh( materials);
    return new_mesh;
}

Graphics::Materials* SharedGraphics::createEmptyMaterials()
{
    m_materialsList.append( Graphics::Materials());
    return &m_materialsList.last();
}

Graphics::Mesh* SharedGraphics::getMesh(int meshId)
{
    return m_meshes[ meshId];
}

void SharedGraphics::createVBOFromMesh(Graphics::Mesh* mesh, VBOUsage usage, VBOUsage indexUsage, VBOConfig vboConfig)
{
    // Not implemented yet: From index in VBO.

    if (vboConfig == OneVBOPerAttrib)
    {
        mesh->m_vboConfig = OneVBOPerAttrib;

        for (GLuint i = 0; i < mesh->m_attribCount; ++i)
        {
            mesh->m_vbos.append(0);
            mesh->m_vboAttribsStartOffset.append(0);

            glGenBuffers( 1, &mesh->m_vbos[i]);
            glBindBuffer( GL_ARRAY_BUFFER, mesh->m_vbos[i]);
            glBufferData( GL_ARRAY_BUFFER, mesh->m_attribSizes.at(i) * sizeof(GLfloat), mesh->m_attribData.at(i), getVBOUsage(usage));
        }
    }
    else if (vboConfig == FromVBOStart_SequentialAttribs || vboConfig == FromVBOStart_InterleavedAttribs)
    {
        mesh->m_vbos.append(0);
        GLuint buffer = 0;
        glGenBuffers( 1, &buffer);
        mesh->m_vbos[0] = buffer;
        glBindBuffer( GL_ARRAY_BUFFER, mesh->m_vbos[0]);

        int total_size = 0;

        for (GLuint& size : mesh->m_attribSizes)
            total_size += size;

        GLfloat* combinedAttributes = new GLfloat[total_size];

        if (vboConfig == FromVBOStart_SequentialAttribs)
        {
            mesh->m_vboConfig = FromVBOStart_SequentialAttribs;

            int output_offset = 0;

            for (GLuint i = 0; i < mesh->m_attribCount; ++i)
            {
                mesh->m_vboAttribsStartOffset.append( output_offset);
                mesh->m_vboAttribsStride = 0;                                       // There is no stride in this configuration.

                for (GLuint j = 0; j < mesh->m_attribSizes.at(i); ++j, ++output_offset)
                    combinedAttributes[output_offset] = mesh->m_attribData.at(i)[j];
            }
        } else {
            mesh->m_vboConfig = FromVBOStart_InterleavedAttribs;

            int output_offset_start = 0;                    // After an attribute has been completely copied to 'combinedAttributes', we increasingly start further from the beginning.

            for (GLuint i = 0; i < mesh->m_attribCount; ++i)
            {
                // Copy ALL values from ONE attribute into 'combinedAttributes'. However, make space between values, for the other attributes.

                int attrib_components = mesh->m_attribComponents.at(i);

                int stride = 0;                                                 // The distance between each set of values in 'combinedAttributes' (depends on number of used attributes).
                for (GLuint& components : mesh->m_attribComponents)
                    stride += components;
                stride -= attrib_components;                                    // The amount to stride is the amount of spaces between this set of values and the next.

                mesh->m_vboAttribsStartOffset.append( output_offset_start);
                mesh->m_vboAttribsStride += stride;                             // Fx. stride 2 for positions, stride 3 for texture coordinates = stride 5.

                for (GLuint attrib_offset = 0, output_offset = output_offset_start;
                     attrib_offset < mesh->m_attribSizes.at(i);
                     attrib_offset += mesh->m_attribComponents.at(i), output_offset += stride)
                {
                    // Copy one set of components from the attribute, then stride.
                    // Fx: Copy 3 numbers from positions, then make space for 3 normal numbers and 2 texcoord numbers.

                    for (int j = 0; j < attrib_components; ++j, ++output_offset)
                        combinedAttributes[output_offset] = mesh->m_attribData.at(i)[attrib_offset + j];
                }

                output_offset_start += attrib_components;
            }
        }
        glBufferData( GL_ARRAY_BUFFER, total_size * sizeof(GLfloat), combinedAttributes, getVBOUsage(usage));
        qDebug() << "VBO of size" << total_size << "was created.";
        delete combinedAttributes;
    } else {
        qWarning() << "Create VBO error: the vbo wasn't created, because you've used a wrong vboConfig.";
    }

    // (indices can't be interleaved with the other attributes)     // --- make a big combined vbo for indices instead.
    if (mesh->m_useIndices)
    {
        glGenBuffers( 1, &mesh->m_vboForIndices);
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->m_vboForIndices);
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, mesh->m_indicesSize * sizeof(GLuint), mesh->m_indices, getVBOUsage(indexUsage));
    }

    // FREE MEMORY FROM MESH. It's in the vbo, so we don't need it any more.
    mesh->freeAttribData();
}

bool SharedGraphics::initSDL()
{
    qDebug() << "Initializing SDL...";
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        Shared::global.endMessage = "Error: Could not initiate SDL.";
        return false;
    } else {
        SDL_version compiled;
        SDL_VERSION(&compiled);
        QString version;
        QTextStream(&version) << compiled.major << '.' << compiled.minor << '.' << compiled.patch;
        qDebug() << qPrintable("SDL v" + version + " was successfully initialized.");
        return true;
    }
}

void SharedGraphics::applyAntiAlias()
{
    if (Shared::global.jsonSettings->value({"msaa"}).toBool())
    {
        int amount_of_msaa = Shared::global.jsonSettings->value({"msaa_level"}).toInt();
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, amount_of_msaa);
        glEnable(GL_MULTISAMPLE);
    } else {
        glDisable(GL_MULTISAMPLE);
    }
}

void SharedGraphics::initGL()
{
    glShadeModel(GL_SMOOTH);                                                                // Enables Smooth Shading [DON't KNOW WHAT THIS IS YET]
    glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.0);                           // Set clear color.
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);                                                                // Enables Depth Testing (depth test mode is set in mesh)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);                                      // Really Nice Perspective Calculations
}

void SharedGraphics::preRenderIndexVBOSetup( Graphics::Mesh* mesh)
{
    if (mesh->m_useIndices && mesh->m_vboForIndices != boundVBOBufferForIndices)
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh->m_vboForIndices);
        boundVBOBufferForIndices = mesh->m_vboForIndices;
    }
}

void SharedGraphics::drawCommand( Graphics::Mesh* mesh, Graphics::MaterialRange* range, int elementsToDrawNow)
{
    glDrawArrays( mesh->m_currentDrawMode,
                  range->indexStart,
                  elementsToDrawNow);
}

void SharedGraphics::renderWithoutVBO( Graphics::Mesh* mesh)
{
    QList<GLuint> boundLocations;

    // SETUP ATTRIBUTE POINTERS.
    for (GLuint i = 0; i < mesh->m_attribCount; ++i)
    {
        boundLocations.append( shaders.getAttributeLocation( mesh->m_attribNames.at(i).constData()));
        glEnableVertexAttribArray( boundLocations.last());
        glVertexAttribPointer( boundLocations.last(), mesh->m_attribComponents.at(i), GL_FLOAT, GL_FALSE, 0, mesh->m_attribData.at(i));
    }

    // DRAW EACH MATERIAL.
    for (int j = 0; j < mesh->m_materialRanges.list.size(); ++j)
    {
        Graphics::MaterialRange* range = &mesh->m_materialRanges.list[j];
        mesh->m_materials->applyMaterial( range->materialName);
        int elements_to_draw_now = range->indexEnd - range->indexStart + 1;
        drawCommand( mesh, range, elements_to_draw_now);
    }

    // DISABLE ATTRIBUTE ARRAYS.
    for (GLuint& location : boundLocations)
        glDisableVertexAttribArray(location);
}

void SharedGraphics::render( Graphics::Mesh* mesh)
{
    if (!mesh->m_loaded)
        return;

    mesh->saveMatrix();                       // Save rotation and translation as it is now.
    mesh->applyTransform2();                  // Applies rotation and translation as it is in this object.
    mesh->applyTransformInOpenGL();           // Finally tells OpenGL how the matrix is, before drawing.
    mesh->applyCullingMode();

    if (mesh->m_vboConfig == NoVBO)
    {
        renderWithoutVBO( mesh);
        mesh->loadMatrix();                   // Resets the rotation and translation to what it was when it was saved.
        return;
    }

    QList<GLuint> boundLocations;

    // BIND INDICES (if applicable)
    preRenderIndexVBOSetup( mesh);

    // BIND THE VBO.
    if (mesh->m_vboConfig == FromVBOStart_InterleavedAttribs || mesh->m_vboConfig == FromVBOStart_SequentialAttribs )
        glBindBuffer( GL_ARRAY_BUFFER, mesh->m_vbos[0]);

    // SETUP ATTRIBUTE POINTERS.
    for (GLuint i = 0; i < mesh->m_attribCount; ++i)
    {
        boundLocations.append( shaders.getAttributeLocation( mesh->m_attribNames.at(i).constData()));
        glEnableVertexAttribArray( boundLocations.last());

        if (mesh->m_vboConfig == OneVBOPerAttrib)               // Bind one VBO for each attribute, if that was the chosen setting.
            glBindBuffer( GL_ARRAY_BUFFER, mesh->m_vbos[i]);

        glVertexAttribPointer( boundLocations.last(), mesh->m_attribComponents.at(i), GL_FLOAT, GL_FALSE,
                               mesh->m_vboAttribsStride * sizeof(GLfloat),
                               reinterpret_cast<GLvoid*>( mesh->m_vboAttribsStartOffset.at(i) * sizeof(GLfloat)));
    }

    // DRAW EACH MATERIAL.
    for (int j = 0; j < mesh->m_materialRanges.list.size(); ++j)
    {
        Graphics::MaterialRange* range = &mesh->m_materialRanges.list[j];
        mesh->m_materials->applyMaterial( range->materialName);                 // This isn't the most efficient way to do it.
        int elements_to_draw_now = range->indexEnd - range->indexStart + 1;
        drawCommand( mesh, range, elements_to_draw_now);
    }

    // DISABLE ATTRIBUTE ARRAYS.
    for (GLuint& location : boundLocations)
        glDisableVertexAttribArray(location);

    // UNBIND THE VBOs.
    glBindBuffer( GL_ARRAY_BUFFER, 0);

    // UNBIND INDICES.
    if (mesh->m_useIndices)
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh->loadMatrix();                       // Resets the rotation and translation to what it was when it was saved.
}

int SharedGraphics::getVBOUsage( VBOUsage usage)
{
    switch(usage)
    {
    case VBOUsage::StreamDraw:
        return GL_STREAM_DRAW;
    case VBOUsage::StreamRead:
        return GL_STREAM_READ;
    case VBOUsage::StreamCopy:
        return GL_STREAM_COPY;
    case VBOUsage::StaticDraw:
        return GL_STATIC_DRAW;
    case VBOUsage::StaticRead:
        return GL_STATIC_READ;
    case VBOUsage::StaticCopy:
        return GL_STATIC_COPY;
    case VBOUsage::DynamicDraw:
        return GL_DYNAMIC_DRAW;
    case VBOUsage::DynamicRead:
        return GL_DYNAMIC_READ;
    case VBOUsage::DynamicCopy:
        return GL_DYNAMIC_COPY;
    default: return GL_STATIC_COPY;
    }
}

SharedGraphics graphics;
}
