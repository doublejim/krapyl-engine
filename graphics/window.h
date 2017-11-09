#ifndef GWINDOW_H
#define GWINDOW_H

#include <QCoreApplication>
#include <QProcess>
#include <QDir>
#include <QDebug>

#include "sdl_components.h"
#include "layer.h"

#include "input/inputs.h"
#include "matrix_transform.h"
#include "matrix.h"
#include "point.h"
#include "fps_manager.h"

namespace Graphics {
class Layer;

class Window : public QObject
{
    Q_OBJECT
private:
    // SETTINGS
    QString m_windowTitle = 0;
    int m_windowWidth = 0;
    int m_windowHeight = 0;
    bool m_isFullScreen = false;
    bool m_hasBorders = true;
    bool m_vsync = false;
    //
    SDL_Window* m_window = nullptr;
    int m_currentDrawLayer = 0;

    SDL_Event m_sdlEvent;
    QList<Layer*> m_layers;
    Input::InputEvent m_input;
    MatrixTransform* m_matrixTransform = nullptr;
    void initGLContext();

public:
    Window( QString title, int width, int height);
    ~Window();

    void setVsyncEnabled( bool value);
    void setBordersEnabled( bool value);
    void setFullScreenEnabled( bool value);
    void setResolution( int width, int height);
    void setTitle( QString title);
    void createWindow();
    void clearAndRender();
    Layer2D* createEmptyLayer2D();
    Layer3D* createEmptyLayer3D();
    void insertLayer(Layer *layer);
    void renderOneFrame();
    void SDLSwapBuffers();
};
}
#endif // GWINDOW_H
