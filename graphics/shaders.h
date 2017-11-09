#ifndef GSHADER
#define GSHADER

#include "sdl_components.h"
#include "color.h"
#include "shared/global_shared.h"
#include <QString>

namespace Graphics {
class Shaders
{
public:
    QMap<QString, GLuint> m_programs;
    GLuint m_activeProgram = 0;

    bool createShaderProgram( QString nameOfProgram, QString vertexShaderFile, QString fragmentShaderFile);
    GLuint createShader( GLenum shaderType, QString fileName);
    void useProgram( QString name);
    void useProgram( GLuint program);
    void endOfProgramUse();
    GLuint getProgram(QString name);
    void setUniformVec4(const char *name, Color &color, float &transparency);
    void setUniformFloat( const char *name, float value);
    void setUniformFloat(const char *programName, const char *name, float value);
    void setUniformInteger(const char *name, float value);
    GLint getAttributeLocation(const char *name);
};
}

#endif // GSHADER
