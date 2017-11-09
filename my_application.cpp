#include "my_application.h"

MyApplication::MyApplication()
{
    Shared::global.init();
    Shared::global.setApplicationPath( m_applicationPath );
    Shared::global.jsonSettings->loadFile( Shared::global.applicationPath + "settings.json");
    Shared::global.jsonKeyboard->loadFile( Shared::global.applicationPath + "config.json");
    Shared::global.resolutionW = Shared::global.jsonSettings->value({"window_w"}).toInt();
    Shared::global.resolutionH = Shared::global.jsonSettings->value({"window_h"}).toInt();

    // Defaults
    if (!Shared::global.jsonSettings->exists({"mipmap_allowed"}))
        Shared::global.jsonSettings->setValue( {"mipmap_allowed"}, true);

    if (!Shared::global.jsonSettings->exists({"filtering"}))
        Shared::global.jsonSettings->setValue( {"filtering"}, "trilinear");

    if (!Shared::global.jsonSettings->exists({"vsync"}))
        Shared::global.jsonSettings->setValue( {"vsync"}, true);

    if (!Shared::global.jsonSettings->exists({"fps_limit"}))
        Shared::global.jsonSettings->setValue( {"fps_limit"}, 60);

    if (!Shared::global.jsonSettings->exists({"window_w"}))
        Shared::global.jsonSettings->setValue( {"window_w"}, 800);

    if (!Shared::global.jsonSettings->exists({"window_h"}))
        Shared::global.jsonSettings->setValue( {"window_h"}, 600);

    if (!Shared::global.jsonSettings->exists({"window_borders"}))
        Shared::global.jsonSettings->setValue( {"window_borders"}, true);

    if (!Shared::global.jsonSettings->exists({"fullscreen"}))
        Shared::global.jsonSettings->setValue( {"fullscreen"}, false);

    if (!Shared::global.jsonSettings->exists({"msaa"}))
        Shared::global.jsonSettings->setValue( {"msaa"}, true);

    if (!Shared::global.jsonSettings->exists({"msaa_level"}))
        Shared::global.jsonSettings->setValue( {"msaa_level"}, 2);
}

MyApplication::~MyApplication()
{    
    qDebug() << "destroying my application";
    qDebug() << "end message:" << Shared::global.endMessage;
    Shared::global.jsonSettings->save(JsonWax::StringStyle::Readable);
    Shared::global.jsonKeyboard->save(JsonWax::StringStyle::Readable);
    Shared::graphics.deleteOpenGLContext();
}

void MyApplication::createWindow()
{
    Window* window = new Window( "My Application",
                                 Shared::global.resolutionW,
                                 Shared::global.resolutionH);

    window->setBordersEnabled( Shared::global.jsonSettings->value({"window_borders"}).toBool());
    window->setFullScreenEnabled( Shared::global.jsonSettings->value({"fullscreen"}).toBool());
    window->setVsyncEnabled( Shared::global.jsonSettings->value({"vsync"}).toBool());
    window->createWindow();

    m_windows.append( window);

    // So far it's not easy to have more than one scene.
}

void MyApplication::stop()
{
    for (int i = 0; i < m_windows.size(); ++i)
    {
        Window* window = m_windows.at(i);
        delete window;
        window = 0;
    }
}

void MyApplication::run()
{
    m_vsync = Shared::global.jsonSettings->value({"vsync"}).toBool();

    // Create a number of windows.
    int number_of_windows = Shared::global.jsonSettings->value({"windows"}).toInt();
    for (int i = 0; i < number_of_windows; ++i)
        createWindow();

    SDL_ShowCursor(SDL_DISABLE);

    // Create a layer.
    Layer* layer0 = new Layer3D( Shared::global.resolutionW, Shared::global.resolutionH);
    layer0->setBlendingEnabled(true);
    layer0->setBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // GL_ONE_MINUS_SRC_ALPHA       // GL_SRC_ALPHA GL_SRC_COLOR
    layer0->transform()->translate(-5, 0, 0);

    Layer* layer1 = new Layer3D( Shared::global.resolutionW, Shared::global.resolutionH);

    // Put the created layer into the created windows. Now all windows use the same layer.
    for (Window* window : m_windows)
    {
        window->insertLayer(layer0);
        window->insertLayer(layer1);
    }

    Scene* scene2 = layer0->createEmptyScene();
    scene2->transform()->translate( 5, -1, -5);
    scene2->transform()->rotate( -10, 0, 0);

    Scene* scene3 = layer1->createEmptyScene();
    scene3->transform()->rotate( 0, 0, 0);

    Shared::graphics.shaders.createShaderProgram("texture_default", "texture_vert.glsl", "texture_frag.glsl");
    Shared::graphics.shaders.createShaderProgram("texture_wave", "texture_wave_vert.glsl", "texture_wave_frag.glsl");

    {
        QImage image(256, 256, QImage::Format_ARGB32);
        image.fill( Qt::transparent);

        QPainter painter( &image);
        painter.setBrush( QBrush(Qt::yellow, Qt::Dense1Pattern) );
        painter.scale(1.0, 1.0);
        painter.drawRect( image.rect());
        Shared::graphics.createTexture( "image0", image, true);

        // I don't like this, but it's better than creating too many VBOs.
        int meshId = Shared::graphics.createTexturedRect( Point(0.0f, 0.0f, 0.0f), Point(3.0f, 3.0f, 0.0f), 10, Color(1.0f, 1.0f, 1.0f), "image0", CullingMode::CullDisabled);

        for (int x = 0; x < 40; ++x)
            for (int y = 0; y < 40; ++y)
            {
                Entity* entity = new Entity();
                Graphics::Mesh* new_mesh = Shared::graphics.getMesh(meshId)->copy();      // WHEN will this be deleted???? That's right, it won't be.
                entity->insertMesh( new_mesh);
                entity->transform()->translate(-4 * x, -1, - 14 - 3 * y);
                entity->setDepthTestMode( Entity::DepthTestLEQUAL );
                entity->setShaderProgram("texture_default");
                scene2->insertEntity( entity);
            }
    }

    {
        Entity* entity = new Entity();
        entity->loadFromObjFile("untitled.obj", true, true, CullingMode::CullDisabled);   // currently you NEED to load both normals and texture coordinates.
        entity->setDepthTestMode( Entity::DepthTestLEQUAL );
        entity->setShaderProgram("texture_wave");
        scene2->insertEntity( entity);
    }

    {
        Entity* entity = new Entity();
        entity->loadFromObjFile("palme.obj", true, true, CullingMode::CullDisabled);   // currently you NEED to load both normals and texture coordinates.
        entity->setDepthTestMode( Entity::DepthTestLEQUAL );
        entity->setShaderProgram("texture_default");
        entity->transform()->translate(-5, -2, 0);
        scene2->insertEntity( entity);
    }

    {
        QImage image(256, 256, QImage::Format_ARGB32);
        image.fill( Qt::transparent);

        QPainter painter( &image);
        painter.setBrush( QBrush(Qt::green, Qt::Dense3Pattern) );
        painter.scale(3.0, 3.0);
        painter.drawRect( image.rect());
        Shared::graphics.createTexture( "image1", image, true);

        // I don't like this, but it's better than creating too many VBOs.
        int meshId = Shared::graphics.createTexturedRect( Point(0.0f, 0.0f, 0.0f), Point(3.0f, 0.0f, 3.0f), 10, Color(1.0f, 1.0f, 1.0f), "image1", CullingMode::CullDisabled);

        for (int x = 0; x < 5; ++x)
            for (int y = 0; y < 5; ++y)
            {
                Entity* entity = new Entity();
                Graphics::Mesh* new_mesh = Shared::graphics.getMesh(meshId)->copy();      // WHEN will this be deleted???? That's right, it won't be.
                entity->insertMesh( new_mesh);
                entity->transform()->translate(-4 * x, -2, -4 * y);
                entity->setDepthTestMode( Entity::DepthTestLEQUAL );
                entity->setShaderProgram("texture_wave");
                scene2->insertEntity( entity);
            }
    }

    QImage image2(256, 256, QImage::Format_ARGB32);

    {
        image2.fill( Qt::transparent);
        Shared::graphics.createTexture( "image2", image2, false);

        Entity* entity = new Entity();
        int meshId2 = Shared::graphics.createTexturedRect( Point(0.0f, 0.0f, 0.0f), Point(1.0f, 1.0f, 0.0f), 1, Color(1.0f, 1.0f, 1.0f), "image2", CullingMode::CullBack);
        Graphics::Mesh* mesh = Shared::graphics.getMesh( meshId2);
        entity->insertMesh( mesh);
        entity->transform()->translate( -2.2f, 0.7f, -3);
        entity->setDepthTestMode( Entity::DepthTestDisabled );  // so that it's never hidden behind other items in the world.
        entity->setShaderProgram("texture_default");
        scene3->insertEntity( entity);
    }

    // Prepare rendering.
    Shared::graphics.initGL();
    FpsManager fps;
    fps.setFpsLimit( Shared::global.jsonSettings->value({"fps_limit"}).toInt());

    // FPS texture.
    GLuint fpsTex = Shared::graphics.textures.value("image2");

    // Mouse scaling.
    Shared::global.mouse.setScaleAmounts( 1.0 * Shared::global.resolutionW, 1.0 * Shared::global.resolutionH);

    while (!Shared::global.closeProgram)
    {
        Shared::graphics.shaders.setUniformFloat( "texture_wave", "u_time", fps.globalTimeMs() / 1000.0);

        // Update FPS display.
        QPainter painter( &image2);
        QRect rect (image2.rect());
        rect.setHeight(30);
        painter.fillRect(rect, Qt::black);
        painter.setPen(Qt::red);
        painter.setFont( QFont("Arial", 20, QFont::Bold));
        painter.drawText( rect, Qt::AlignTop | Qt::AlignLeft, "fps:" + QString::number( fps.currentFPS(), 'f', 2));
        Shared::graphics.updateTexture( fpsTex, image2, rect);

        // Update camera through keyboard input.
        if (Shared::global.keyboard.m_keyStates.value(SDLK_LEFT))
            scene2->transform()->translateXDelta(0.01 * fps.timePerFrameMs());

        if (Shared::global.keyboard.m_keyStates.value(SDLK_RIGHT))
            scene2->transform()->translateXDelta(-0.01 * fps.timePerFrameMs());

        if (Shared::global.keyboard.m_keyStates.value(SDLK_UP))
            scene2->transform()->translateZDelta(0.01 * fps.timePerFrameMs());

        if (Shared::global.keyboard.m_keyStates.value(SDLK_DOWN))
            scene2->transform()->translateZDelta(-0.01 * fps.timePerFrameMs());

        // Update camera through mouse input.
        scene2->transform()->rotateX( (- 0.5 + Shared::global.mouse.m_deltaYzeroToOne)); // mouse move vertical means rotate X
        scene2->transform()->rotateY( (- 0.5 + Shared::global.mouse.m_deltaXzeroToOne)); // mouse move horizontal means rotate Y

        // Render one frame in each window.
        for (Window* window : m_windows)
        {
            window->renderOneFrame();
            window->SDLSwapBuffers();
        }

        fps.frameEnd();

        if (!m_vsync)
            fps.idleToEnforceFpsLimit();
    }
    stop();

    delete scene2;      // This is horrible.
    qWarning() << "Sorry, you'll have to force close from here...";
    //delete this;
}
