#-------------------------------------------------
#
# Project created by QtCreator 2015-08-01T17:31:07
#
#-------------------------------------------------

QT       += core gui

TARGET = krapyl
CONFIG += console
CONFIG -= app_bundle
CONFIG += C++11

TEMPLATE = app

LIBS += -L"$$_PRO_FILE_PWD_/lib64" -lSDL2 -lSDL2main -lglew32 -lglu32 -lopengl32
INCLUDEPATH += "$$_PRO_FILE_PWD_/include/"

DISTFILES += \
    ../engineData/shaders/texture_wave_frag.glsl \
    ../engineData/shaders/texture_wave_vert.glsl

HEADERS += \
    graphics/color.h \
    graphics/entities.h \
    graphics/entity.h \
    graphics/fps_manager.h \
    graphics/layer.h \
    graphics/materials.h \
    graphics/matrix.h \
    graphics/matrix_transform.h \
    graphics/mesh.h \
    graphics/mesh_factory.h \
    graphics/mesh_loader.h \
    graphics/point.h \
    graphics/scene.h \
    graphics/sdl_components.h \
    graphics/shaders.h \
    graphics/window.h \
    my_application.h \
    input/inputs.h \
    shared/file_reader.h \
    shared/global_shared.h \
    shared/graphics_shared.h \
    shared/thread_pool.h \
    input/keyboard.h \
    graphics/vbo_manager.h \
    input/mouse.h

SOURCES += \
    graphics/color.cpp \
    graphics/entities.cpp \
    graphics/entity.cpp \
    graphics/fps_manager.cpp \
    graphics/layer.cpp \
    graphics/materials.cpp \
    graphics/matrix.cpp \
    graphics/matrix_transform.cpp \
    graphics/mesh.cpp \
    graphics/mesh_factory.cpp \
    graphics/mesh_loader.cpp \
    graphics/point.cpp \
    graphics/scene.cpp \
    graphics/shaders.cpp \
    graphics/window.cpp \
    main.cpp \
    my_application.cpp \
    input/inputs.cpp \
    shared/file_reader.cpp \
    shared/global_shared.cpp \
    shared/graphics_shared.cpp \
    shared/thread_pool.cpp \
    input/keyboard.cpp \
    graphics/vbo_manager.cpp \
    input/mouse.cpp
