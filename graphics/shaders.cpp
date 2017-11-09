#include "shaders.h"
namespace Graphics {
bool Shaders::createShaderProgram(QString nameOfProgram, QString vertexShaderFile, QString fragmentShaderFile)
{
    GLuint program;
    program = glCreateProgram();

    GLuint vertexShader = createShader( GL_VERTEX_SHADER, vertexShaderFile);
    GLuint fragmentShader = createShader( GL_FRAGMENT_SHADER, fragmentShaderFile);

    if (vertexShader == -1 || fragmentShader == -1)
    {
        return false;
    }

    glAttachShader( program, vertexShader);
    glAttachShader( program, fragmentShader);
    glLinkProgram( program);

    GLint status = GL_FALSE;
    glGetProgramiv( program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength = 500;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar strInfoLog[4096];
        glGetProgramInfoLog( program, infoLogLength, NULL, strInfoLog);
        qDebug() << "Failed when creating shader program:" << strInfoLog;
        return false;
    }

    qDebug() << "Created shader program successfully!";
    m_programs.insert( nameOfProgram, program);
    return true;
}

GLuint Shaders::createShader( GLenum shaderType, QString fileName)
{
    QString text = Shared::global.fileReader.readTextFile("shaders/" + fileName);

    QByteArray shaderSrcBytes = text.toLatin1();
    const char* shaderSource = shaderSrcBytes.data();

    GLuint shader = glCreateShader( shaderType);
    glShaderSource( shader, 1, &shaderSource, NULL);
    glCompileShader( shader);

    GLint isOkay = GL_FALSE;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &isOkay);
    if (isOkay == GL_FALSE)
    {
        GLint infoLogLength = 500;
        GLchar strInfoLog[4096];
        GLsizei sizei = 200;
        glGetShaderInfoLog( shader, infoLogLength, &sizei, strInfoLog);
        qDebug() << "Error in shader:" << fileName;
        printf( "%s\n", strInfoLog);
        qDebug() << "Bad source code:" << text;
        glDeleteShader( shader);
        shader = -1;
    }
    return shader;
}

void Shaders::useProgram( QString name)
{
    m_activeProgram = m_programs.value( name, 0);
    glUseProgram( m_programs.value( name));
}

void Shaders::useProgram(GLuint program)
{
    m_activeProgram = program;
    glUseProgram( program);
}

void Shaders::endOfProgramUse()
{
    glUseProgram(0);
}

GLuint Shaders::getProgram(QString name)
{
    GLuint program = m_programs.value( name, 0);

    if (program == 0)
        qWarning() << "Shader program:" << qPrintable(name) << "doesn't exist!";
    return program;
}

void Shaders::setUniformVec4( const char* name, Color& color, float& transparency)
{
    GLint location = glGetUniformLocation( m_activeProgram, name);

    if (location == -1)
    {
        qWarning() << "Uniform:" << name << "wasn't found in the active program.";
        return;
    }

    glUniform4f( location, color.r, color.g, color.b, transparency);
}

void Shaders::setUniformFloat(const char *name, float value)
{
    GLint location = glGetUniformLocation( m_activeProgram, name);

    if (location == -1)
    {
        qWarning() << "Uniform:" << name << "wasn't found in the active program.";
        return;
    }

    glUniform1f( location, value);
}

void Shaders::setUniformInteger(const char *name, float value)
{
    GLint location = glGetUniformLocation( m_activeProgram, name);

    if (location == -1)
    {
        qWarning() << "Uniform:" << name << "wasn't found in the active program.";
        return;
    }

    glUniform1i( location, value);
}

void Shaders::setUniformFloat(const char* programName, const char* name, float value)
{
    useProgram( programName);
    setUniformFloat( name, value);
    endOfProgramUse();
}

GLint Shaders::getAttributeLocation(const char* name)
{
    GLint location = glGetAttribLocation( m_activeProgram, name);
    if (location == -1)
    {
        // qWarning() << "Attribute:" << name << "wasn't found in the active program.";
        return -1;
    }
    return location;
}

}
