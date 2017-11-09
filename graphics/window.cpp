#include "window.h"
namespace Graphics {
Window::Window(QString title, int width, int height)
{
    m_windowTitle = title;
    m_windowWidth = width;
    m_windowHeight = height;
}

Window::~Window()
{
    qDebug() << "deleting window" << this;
    SDL_DestroyWindow( m_window);

    // Delete all the layers (which in turn will delete the scenes (but not the entities and below))
    // THIS SHOULD BE CHANGED. all the scenes must be deleted separately instead.

    for (Layer* layer : m_layers)
    {
        qDebug() << "deleting layer:" << layer;
        delete layer;
        layer = 0;
    }
}

void Window::createWindow()
{
    // ======== INITIALIZE SDL IF IT'S NOT ========
    if (!Shared::graphics.sdlInitialized)
    {
        if (Shared::graphics.initSDL())
            Shared::graphics.sdlInitialized = true;
        else
        {
            qWarning() << "Error: SDL could not be initialized!";
            return;
        }
    }

    // Destroy window if one already exists.
    if (m_window != nullptr)
        SDL_DestroyWindow(m_window);

    // ======== APPLY ANTI ALIAS ========
    Shared::graphics.applyAntiAlias();

    // ======== PREPARE FOR OPENGL CONTEXT SHARING (if there will be more than one window) ========
    if (Shared::global.jsonSettings->value({"windows"}).toInt() > 1 && Shared::graphics.sdlOpenGLContext == nullptr)
    {
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
    } else {
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    }

    // ======== CREATE THE WINDOW ========
    m_window = SDL_CreateWindow( m_windowTitle.toLocal8Bit(),
               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
               m_windowWidth, m_windowHeight,
               SDL_WINDOW_OPENGL);          //| SDL_WINDOW_SHOWN

    if (m_window == 0)
    {
        Shared::global.endMessage = SDL_GetError();
        return;
    }

    SDL_SetWindowBordered( m_window, SDL_bool(m_hasBorders));

    // ======== FINALIZE OPENGL CONTEXT SHARING ========
    if (Shared::graphics.sdlOpenGLContext == nullptr)
    {
        initGLContext();
    } else {
        SDL_GL_MakeCurrent( m_window, Shared::graphics.sdlOpenGLContext);
    }

    if (m_isFullScreen)
        SDL_SetWindowFullscreen( m_window, SDL_WINDOW_FULLSCREEN);
}

void Window::initGLContext()
{    
    Shared::graphics.sdlOpenGLContext = SDL_GL_CreateContext( m_window);
    SDL_GL_MakeCurrent( m_window, Shared::graphics.sdlOpenGLContext);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (Shared::graphics.sdlOpenGLContext == nullptr)
    {
        Shared::global.endMessage = SDL_GetError();
        return;
    };

    int vsync = m_vsync ? 1 : 0;
    if (SDL_GL_SetSwapInterval(vsync) < 0)             // vsync
    {
        Shared::global.endMessage = SDL_GetError();
        return;
    };

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1);

    //glewExperimental = GL_TRUE;       // it seems not necessary any more.
    GLenum glew_status = glewInit();

    if (glew_status != GLEW_OK)
    {
        fprintf (stderr, "GLEW ERROR: %s\n", glewGetErrorString(glew_status));
    }
}

void Window::clearAndRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Layer::LayerType lastLayerType = Layer::LayerTypeNull;

    for (Layer* layer : m_layers)
    {
        if (lastLayerType != layer->m_layerType)            // only prepare the layer if it's of a different type than the previous.
            layer->preRender();
        layer->render();
        lastLayerType = layer->m_layerType;
    }
}

void Window::SDLSwapBuffers()
{
    SDL_GL_SwapWindow( m_window );      // Flush to screen and swap the buffers.
}

Layer2D* Window::createEmptyLayer2D()
{
    Layer2D* newLayer = new Layer2D( m_windowWidth, m_windowHeight);
    m_layers.append( newLayer );
    return newLayer;
}

Layer3D* Window::createEmptyLayer3D()
{
    Layer3D* newLayer = new Layer3D( m_windowWidth, m_windowHeight);
    m_layers.append( newLayer );
    return newLayer;
}

void Window::insertLayer( Layer* layer)
{
    m_layers.append( layer);
}

void Window::setVsyncEnabled(bool value)
{
    m_vsync = value;
}

void Window::setBordersEnabled(bool value)
{
    m_hasBorders = value;
}

void Window::setFullScreenEnabled(bool value)
{
    m_isFullScreen = value;
}

void Window::setResolution( int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

void Window::setTitle(QString title)
{
    m_windowTitle = title;
}

void Window::renderOneFrame()
{
    SDL_GL_MakeCurrent( m_window, Shared::graphics.sdlOpenGLContext);

    if (SDL_PollEvent( &m_sdlEvent) != 0)
        m_input.handle( m_sdlEvent);

    //qApp->eventFilter( );

    clearAndRender();
}
}
